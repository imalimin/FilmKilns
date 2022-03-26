// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef PIEX_BINARY_PARSE_RANGE_CHECKED_BYTE_PTR_H_
#define PIEX_BINARY_PARSE_RANGE_CHECKED_BYTE_PTR_H_

#include <assert.h>

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace piex {
namespace binary_parse {

// Since NaCl does not comply to C++11 we can not just use stdint.h.
typedef unsigned short uint16;  // NOLINT
typedef short int16;            // NOLINT
typedef unsigned int uint32;
typedef int int32;

enum MemoryStatus {
  RANGE_CHECKED_BYTE_SUCCESS = 0,
  RANGE_CHECKED_BYTE_ERROR = 1,
  RANGE_CHECKED_BYTE_ERROR_OVERFLOW = 2,
  RANGE_CHECKED_BYTE_ERROR_UNDERFLOW = 3,
};

// Interface that RangeCheckedBytePtr uses to access the underlying array of
// bytes. This allows RangeCheckedBytePtr to be used to access data as if it
// were stored contiguously in memory, even if the data is in fact split up
// into non-contiguous chunks and / or does not reside in memory.
//
// The only requirement is that the data can be read in pages of a fixed (but
// configurable) size. Notionally, the byte array (which contains length()
// bytes) is split up into non-overlapping pages of pageSize() bytes each.
// (The last page may be shorter if length() is not a multiple of pageSize().)
// There are therefore (length() - 1) / pageSize() + 1 such pages, with indexes
// 0 through (length() - 1) / pageSize(). Page i contains the bytes from offset
// i * pageSize() in the array up to and including the byte at offset
// (i + 1) * pageSize() - 1 (or, in the case of the last page, length() - 1).
//
// In essence, RangeCheckedBytePtr and PagedByteArray together provide a poor
// man's virtual-memory-and-memory-mapped-file work-alike in situations where
// virtual memory cannot be used or would consume too much virtual address
// space.
//
// Thread safety: In general, subclasses implementing this interface should
// ensure that the member functions are thread-safe. It will then be safe to
// access the same array from multiple threads. (Note that RangeCheckedBytePtr
// itself is not thread-safe in the sense that a single instance of
// RangeCheckedBytePtr cannot be used concurrently from multiple threads; it
// is, however, safe to use different RangeCheckedBytePtr instances in
// different threads to access the same PagedByteArray concurrently, assuming
// that the PagedByteArray implementation is thread-safe.)
class PagedByteArray {
 public:
  // Base class for pages in the byte array. Implementations of PagedByteArray
  // can create a subclass of the Page class to manage the lifetime of buffers
  // associated with a page returned by getPage(). For example, a
  // PagedByteArray backed by a file might define a Page subclass like this:
  //
  //  class FilePage : public Page {
  //    std::vector<unsigned char> bytes;
  //  };
  //
  // The corresponding getPage() implementation could then look like this:
  //
  //   void getPage(size_t page_index, const unsigned char** begin,
  //       const unsigned char** end, std::shared_ptr<Page>* page)
  //       {
  //     // Create a new page.
  //     std::shared_ptr<FilePage> file_page(new FilePage());
  //
  //     // Read contents of page from file into file_page->bytes.
  //     [...]
  //
  //     // Set *begin and *end to point to beginning and end of
  //     // file_page->bytes vector.
  //     *begin = &file_page->bytes[0];
  //     *end = *begin + file_page->bytes.size();
  //
  //     // Return page to caller
  //     *page = file_page;
  //   }
  //
  // In this way, the storage associated with the page (the FilePage::bytes
  // vector) will be kept alive until the RangeCheckedBytePtr releases the
  // shared pointer.
  class Page {};

  typedef std::shared_ptr<Page> PagePtr;

  virtual ~PagedByteArray();

  // Returns the length of the array in bytes. The value returned must remain
  // the same on every call for the entire lifetime of the object.
  virtual size_t length() const = 0;

  // Returns the length of each page in bytes. (The last page may be shorter
  // than pageSize() if length() is not a multiple of pageSize() -- see also
  // the class-wide comment above.) The value returned must remain the same on
  // every call for the entire lifetime of the object.
  virtual size_t pageSize() const = 0;

  // Returns a pointer to a memory buffer containing the data for the page
  // with index "page_index".
  //
  // *begin is set to point to the first byte of the page; *end is set to point
  // one byte beyond the last byte in the page. This implies that:
  // - (*end - *begin) == pageSize() for every page except the last page
  // - (*end - *begin) == length() - pageSize() * ((length() - 1) / pageSize())
  //   for the last page
  //
  // *page will be set to a SharedPtr that the caller will hold on to until
  // it no longer needs to access the memory buffer. The memory buffer will
  // remain valid until the SharedPtr is released or the PagedByteArray object
  // is destroyed. An implementation may choose to return a null SharedPtr;
  // this indicates that the memory buffer will remain valid until the
  // PagedByteArray object is destroyed, even if the caller does not hold on to
  // the SharedPtr. (This is intended as an optimization that some
  // implementations may choose to take advantage of, as a null SharedPtr is
  // cheaper to copy.)
  virtual void getPage(size_t page_index, const unsigned char **begin,
                       const unsigned char **end, PagePtr *page) const = 0;
};

typedef std::shared_ptr<PagedByteArray> PagedByteArrayPtr;

// Smart pointer that has the same semantics as a "const unsigned char *" (plus
// some convenience functions) but provides range checking and the ability to
// access arrays that are not contiguous in memory or do not reside entirely in
// memory (through the PagedByteArray interface).
//
// In the following, we abbreviate RangeCheckedBytePtr as RCBP.
//
// The intent of this class is to allow easy security hardening of code that
// parses binary data structures using raw byte pointers. To do this, only the
// declarations of the pointers need to be changed; the code that uses the
// pointers can remain unchanged.
//
// If an illegal operation occurs on a pointer, an error flag is set, and all
// read operations from this point on return 0. This means that error checking
// need not be done after every access; it is sufficient to check the error flag
// (using errorOccurred()) once before the RCBP is destroyed. Again, this allows
// the majority of the parsing code to remain unchanged. (Note caveats below
// that apply if a copy of the pointer is created.)
//
// Legal operations are exactly the ones that would be legal on a raw C++
// pointer. Read accesses are legal if they fall within the underlying array. A
// RCBP may point to any element in the underlying array or one element beyond
// the end of the array.
//
// For brevity, the documentation for individual member functions does not state
// explicitly that the error flag will be set on out-of-range operations.
//
// Note:
//
// - Just as for raw pointers, it is legal for a pointer to point one element
//   beyond the end of the array, but it is illegal to use operator*() on such a
//   pointer.
//
// - If a copy of an RCBP is created, then performing illegal operations on the
//   copy affects the error flag of the copy, but not of the original pointer.
//   Note that using operator+ and operator- also creates a copy of the pointer.
//   For example:
//
//     // Assume we have an RCBP called "p" and a size_t variable called
//     // "offset".
//     RangeCheckedBytePtr sub_data_structure = p + offset;
//
//   If "offset" is large enough to cause an out-of-range access, then
//   sub_data_structure.errorOccurred() will be true, but p.errorOccurred() will
//   still be false. The error flag for sub_data_structure therefore needs to be
//   checked before it is destroyed.
class RangeCheckedBytePtr {
 private:
  // This class maintains the following class invariants:
  // - page_data_ always points to a buffer of at least current_page_len_
  //   bytes.
  //
  // - The current position lies within the sub-array, i.e.
  //   sub_array_begin_ <= current_pos_ <= sub_array_end_
  //
  // - The sub-array is entirely contained within the array, i.e.
  //   0 <= sub_array_begin <= sub_array_end <= array_->length()
  //
  // - If the current page is non-empty, it lies completely within the
  //   sub-array, i.e.
  //   if _current_page_len_ >= 0, then
  //   sub_array_begin_ <= page_begin_offset_
  //   and
  //   page_begin_offset_ + current_page_len_ <= sub_array_end_
  //   (See also restrictPageToSubArray().)
  //   (If _current_page_len_ == 0, then page_begin_offset_ may lie outside
  //   the sub-array; this condition is harmless. Additional logic would be
  //   required to make page_begin_offset_ lie within the sub-array in this
  //   case, and it would serve no purpose other than to make the invariant
  //   slightly simpler.)
  //
  // Note that it is _not_ a class invariant that current_pos_ needs to lie
  // within the current page. Making this an invariant would have two
  // undesirable consequences:
  // a) When operator[] is called with an index that lies beyond the end of
  //    the current page, it would need to temporarily load the page that
  //    contains this index, but it wouldn't be able to "retain" the page
  //    (i.e. make it the current page) because that would violate the
  //    proposed invariant. This would lead to inefficient behavior in the
  //    case where code accesses a large range of indices beyond the end of
  //    the page because a page would need to be loaded temporarily on each
  //    access.
  // b) It would require more code: loadPageForOffset() would need to be
  //    called anywhere that current_pos_ changes (whereas, with the present
  //    approach, loadPageForOffset() is only called in operator[]).

  // PagedByteArray that is accessed by this pointer.
  PagedByteArrayPtr array_;

  // Pointer to the current page.
  mutable PagedByteArray::PagePtr page_;

  // Pointer to the current page's data buffer.
  mutable const unsigned char *page_data_;

  // All of the following offsets are defined relative to the beginning of
  // the array defined by the PagedByteArray array_.

  // Array offset that the pointer points to.
  size_t current_pos_;

  // Start offset of the current sub-array.
  size_t sub_array_begin_;

  // End offset of the current sub-array.
  size_t sub_array_end_;

  // Array offset corresponding to the "page_data_" pointer.
  mutable size_t page_begin_offset_;

  // Length of the current page.
  mutable size_t current_page_len_;

  // Error flag. This is mutable because methods that don't affect the value
  // of the pointer itself (such as operator[]) nevertheless need to be able to
  // signal error conditions.
  mutable MemoryStatus error_flag_;

  RangeCheckedBytePtr();

 public:
  // Creates a pointer that points to the first element of 'array', which has a
  // length of 'len'. The caller must ensure that the array remains valid until
  // this pointer and any pointers created from it have been destroyed.
  // Note: 'len' may be zero, but 'array' must in this case still be a valid,
  // non-null pointer.
  explicit RangeCheckedBytePtr(const unsigned char *array, const size_t len);

  // Creates a pointer that points to the first element of the given
  // PagedByteArray. The caller must ensure that this PagedByteArray remains
  // valid until this pointer and any pointers created from it have been
  // destroyed.
  explicit RangeCheckedBytePtr(PagedByteArray *array);

  // Creates an invalid RangeCheckedBytePtr. Calling errorOccurred() on the
  // result of invalidPointer() always returns true.
  // Do not check a RangeCheckedBytePtr for validity by comparing against
  // invalidPointer(); use errorOccurred() instead.
  static RangeCheckedBytePtr invalidPointer();

  // Returns a RangeCheckedBytePtr that points to a sub-array of this pointer's
  // underlying array. The sub-array starts at position 'pos' relative to this
  // pointer and is 'length' bytes long. The sub-array must lie within this
  // pointer's array, i.e. pos + length <= remainingLength() must hold. If this
  // condition is violated, an invalid pointer is returned.
  RangeCheckedBytePtr pointerToSubArray(size_t pos, size_t length) const;

  // Returns the number of bytes remaining in the array from this pointer's
  // present position.
  inline size_t remainingLength() const;

  // Returns the offset (or index) in the underlying array that this pointer
  // points to. If this pointer was created using pointerToSubArray(), the
  // offset is relative to the beginning of the sub-array (and not relative to
  // the beginning of the original array).
  size_t offsetInArray() const;

  // Returns whether an out-of-bounds error has ever occurred on this pointer in
  // the past. An error occurs if a caller attempts to read from a position
  // outside the bounds of the array or to move the pointer outside the bounds
  // of the array.
  //
  // The error flag is never reset. Once an error has occurred,
  // all subsequent attempts to read from the pointer (even within the bounds of
  // the array) return 0.
  //
  // Note that it is permissible for a pointer to point one element past the end
  // of the array, but it is not permissible to read from this position. This is
  // equivalent to the semantics of raw C++ pointers.
  inline bool errorOccurred() const;

  // Returns the substring of length 'length' located at position 'pos' relative
  // to this pointer.
  std::string substr(size_t pos, size_t length) const;

  // Returns 'length' number of bytes from the array starting at position 'pos'
  // relative to this pointer.
  std::vector<unsigned char> extractBytes(size_t pos, size_t length) const;

  // Equivalent to calling convert(0, output).
  template <class T>
  bool convert(T *output) const {
    union {
      T t;
      unsigned char ch[sizeof(T)];
    } buffer;
    for (size_t i = 0; i < sizeof(T); i++) {
      buffer.ch[i] = (*this)[i];
    }
    if (!errorOccurred()) {
      *output = buffer.t;
    }
    return !errorOccurred();
  }

  // Reinterprets this pointer as a pointer to an array of T, then returns the
  // element at position 'index' in this array of T. (Note that this position
  // corresponds to position index * sizeof(T) in the underlying byte array.)
  //
  // Returns true if successful; false if an out-of-range error occurred or if
  // the error flag was already set on the pointer when calling convert().
  //
  // The conversion from a sequence of sizeof(T) bytes to a T is performed in an
  // implementation-defined fashion. This conversion is equivalent to the one
  // obtained using the following union by filling the array 'ch' and then
  // reading the member 't':
  //
  //   union {
  //     T t;
  //     unsigned char ch[sizeof(T)];
  //   };
  //
  // Callers should note that, among other things, the conversion is not
  // endian-agnostic with respect to the endianness of T.
  template <class T>
  bool convert(size_t index, T *output) const {
    RangeCheckedBytePtr p = (*this) + index * sizeof(T);
    bool valid = p.convert(output);
    if (!valid) {
      error_flag_ = p.error_flag_;
    }
    return valid;
  }

  // Operators. Unless otherwise noted, these operators have the same semantics
  // as the same operators on an unsigned char pointer.

  // If an out-of-range access is attempted, returns 0 (and sets the error
  // flag).
  inline unsigned char operator[](size_t i) const;

  inline unsigned char operator*() const;

  inline RangeCheckedBytePtr &operator++();

  inline RangeCheckedBytePtr operator++(int);

  inline RangeCheckedBytePtr &operator--();

  inline RangeCheckedBytePtr operator--(int);

  inline RangeCheckedBytePtr &operator+=(size_t x);

  inline RangeCheckedBytePtr &operator-=(size_t x);

  inline friend RangeCheckedBytePtr operator+(const RangeCheckedBytePtr &p,
                                              size_t x);

  inline friend RangeCheckedBytePtr operator-(const RangeCheckedBytePtr &p,
                                              size_t x);

  // Tests whether x and y point at the same position in the underlying array.
  // Two pointers that point at the same position but have different
  // sub-arrays still compare equal. It is not legal to compare two pointers
  // that point into different paged byte arrays.
  friend bool operator==(const RangeCheckedBytePtr &x,
                         const RangeCheckedBytePtr &y);

  // Returns !(x == y).
  friend bool operator!=(const RangeCheckedBytePtr &x,
                         const RangeCheckedBytePtr &y);

 private:
  void loadPageForOffset(size_t offset) const;
  void restrictPageToSubArray() const;
};

// Returns the result of calling std::memcmp() on the sequences of 'num' bytes
// pointed to by 'x' and 'y'. The result is undefined if either
// x.remainingLength() or y.remainingLength() is less than 'num'.
int memcmp(const RangeCheckedBytePtr &x, const RangeCheckedBytePtr &y,
           size_t num);

// Returns the result of calling std::memcmp() (note: _not_ strcmp()) on the
// y.length() number of bytes pointed to by 'x' and the string 'y'. The result
// is undefined if x.remainingLength() is less than y.length().
int strcmp(const RangeCheckedBytePtr &x, const std::string &y);

// Returns the length of the zero-terminated string starting at 'src' (not
// including the '\0' terminator). If no '\0' occurs before the end of the
// array, the result is undefined.
size_t strlen(const RangeCheckedBytePtr &src);

// Integer decoding functions.
//
// These functions read signed (Get16s, Get32s) or unsigned (Get16u, Get32u)
// integers from 'input'. The integer read from the input can be specified to be
// either big-endian (big_endian == true) or little-endian
// (little_endian == false). Signed integers are read in two's-complement
// representation. The integer read in the specified format is then converted to
// the implementation's native integer representation and returned. In other
// words, the semantics of these functions are independent of the
// implementation's endianness and signed integer representation.
//
// If an out-of-range error occurs, these functions do _not_ set the error flag
// on 'input'. Instead, they set 'status' to RANGE_CHECKED_BYTE_ERROR and return
// 0.
//
// Note:
// - If an error occurs and 'status' is already set to an error value (i.e. a
//   value different from RANGE_CHECKED_BYTE_SUCCESS), the value of 'status' is
//   left unchanged.
// - If the operation is successful, 'status' is left unchanged (i.e. it is not
//   actively set to RANGE_CHECKED_BYTE_SUCCESS).
//
// Together, these two properties mean that these functions can be used to read
// a number of integers in succession with only a single error check, like this:
//
//   MemoryStatus status = RANGE_CHECKED_BYTE_SUCCESS;
//   int16 val1 = Get16s(input, false, &status);
//   int32 val2 = Get32s(input + 2, false, &status);
//   uint32 val3 = Get32u(input + 6, false, &status);
//   if (status != RANGE_CHECKED_BYTE_SUCCESS) {
//     // error handling
//   }
int16 Get16s(const RangeCheckedBytePtr &input, const bool big_endian,
             MemoryStatus *status);
uint16 Get16u(const RangeCheckedBytePtr &input, const bool big_endian,
              MemoryStatus *status);
int32 Get32s(const RangeCheckedBytePtr &input, const bool big_endian,
             MemoryStatus *status);
uint32 Get32u(const RangeCheckedBytePtr &input, const bool big_endian,
              MemoryStatus *status);

size_t RangeCheckedBytePtr::remainingLength() const {
  if (!errorOccurred()) {
    // current_pos_ <= sub_array_end_ is a class invariant, but protect
    // against violations of this invariant.
    if (current_pos_ <= sub_array_end_) {
      return sub_array_end_ - current_pos_;
    } else {
      assert(false);
      return 0;
    }
  } else {
    return 0;
  }
}

bool RangeCheckedBytePtr::errorOccurred() const {
  return error_flag_ != RANGE_CHECKED_BYTE_SUCCESS;
}

unsigned char RangeCheckedBytePtr::operator[](size_t i) const {
  // Check that pointer doesn't have an error flag set.
  if (!errorOccurred()) {
    // Offset in array to read from.
    const size_t read_offset = current_pos_ + i;

    // Check for the common case first: The byte we want to read lies in the
    // current page. For performance reasons, we don't check for the case
    // "read_offset < page_begin_offset_" explicitly; if it occurs, it will
    // lead to wraparound (which is well-defined for unsigned quantities), and
    // this will cause the test "pos_in_page < current_page_len_" to fail.
    size_t pos_in_page = read_offset - page_begin_offset_;
    if (pos_in_page < current_page_len_) {
      return page_data_[pos_in_page];
    }

    // Check that the offset we're trying to read lies within the sub-array
    // we're allowed to access.
    if (read_offset >= sub_array_begin_ && read_offset < sub_array_end_) {
      // Read the page that contains the offset "read_offset".
      loadPageForOffset(read_offset);

      // Compute the position within the new page from which we need to read.
      pos_in_page = read_offset - page_begin_offset_;

      // After the call to loadPageForOffset(), read_offset must lie within
      // the current page, and therefore pos_in_page must be less than the
      // length of the page. We nevertheless check for this to protect against
      // potential bugs in loadPageForOffset().
      assert(pos_in_page < current_page_len_);
      if (pos_in_page < current_page_len_) {
        return page_data_[pos_in_page];
      }
    }
  }

// All error cases fall through to here.
#ifdef BREAK_IF_DEBUGGING_AND_OUT_OF_RANGE
  assert(false);
#endif
  error_flag_ = RANGE_CHECKED_BYTE_ERROR_OVERFLOW;
  // return 0, which represents the invalid value
  return static_cast<unsigned char>(0);
}

unsigned char RangeCheckedBytePtr::operator*() const { return (*this)[0]; }

RangeCheckedBytePtr &RangeCheckedBytePtr::operator++() {
  if (current_pos_ < sub_array_end_) {
    current_pos_++;
  } else {
#ifdef BREAK_IF_DEBUGGING_AND_OUT_OF_RANGE
    assert(false);
#endif
    error_flag_ = RANGE_CHECKED_BYTE_ERROR_OVERFLOW;
  }
  return *this;
}

RangeCheckedBytePtr RangeCheckedBytePtr::operator++(int) {
  RangeCheckedBytePtr result(*this);
  ++(*this);
  return result;
}

RangeCheckedBytePtr &RangeCheckedBytePtr::operator--() {
  if (current_pos_ > sub_array_begin_) {
    current_pos_--;
  } else {
#ifdef BREAK_IF_DEBUGGING_AND_OUT_OF_RANGE
    assert(false);
#endif
    error_flag_ = RANGE_CHECKED_BYTE_ERROR_UNDERFLOW;
  }
  return *this;
}

RangeCheckedBytePtr RangeCheckedBytePtr::operator--(int) {
  RangeCheckedBytePtr result(*this);
  --(*this);
  return result;
}

RangeCheckedBytePtr &RangeCheckedBytePtr::operator+=(size_t x) {
  if (remainingLength() >= x) {
    current_pos_ += x;
  } else {
#ifdef BREAK_IF_DEBUGGING_AND_OUT_OF_RANGE
    assert(false);
#endif
    error_flag_ = RANGE_CHECKED_BYTE_ERROR_OVERFLOW;
  }
  return *this;
}

RangeCheckedBytePtr &RangeCheckedBytePtr::operator-=(size_t x) {
  if (x <= current_pos_ - sub_array_begin_) {
    current_pos_ -= x;
  } else {
#ifdef BREAK_IF_DEBUGGING_AND_OUT_OF_RANGE
    assert(false);
#endif
    error_flag_ = RANGE_CHECKED_BYTE_ERROR_UNDERFLOW;
  }
  return *this;
}

RangeCheckedBytePtr operator+(const RangeCheckedBytePtr &p, size_t x) {
  RangeCheckedBytePtr result(p);
  result += x;
  return result;
}

RangeCheckedBytePtr operator-(const RangeCheckedBytePtr &p, size_t x) {
  RangeCheckedBytePtr result(p);
  result -= x;
  return result;
}

}  // namespace binary_parse
}  // namespace piex

#endif  // PIEX_BINARY_PARSE_RANGE_CHECKED_BYTE_PTR_H_
