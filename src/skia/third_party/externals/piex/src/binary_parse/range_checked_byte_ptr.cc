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

#include "src/binary_parse/range_checked_byte_ptr.h"

#include <assert.h>
#include <cstddef>
#include <cstring>

namespace piex {
namespace binary_parse {

#ifdef BREAK_IF_DEBUGGING_AND_OUT_OF_RANGE
#define BREAK_IF_DEBUGGING() assert(false)
#else
#define BREAK_IF_DEBUGGING() assert(true)
#endif

namespace {
class MemoryPagedByteArray : public PagedByteArray {
 public:
  MemoryPagedByteArray(const unsigned char *buffer, const size_t len);

  virtual size_t length() const;
  virtual size_t pageSize() const;
  virtual void getPage(size_t page_index, const unsigned char **begin,
                       const unsigned char **end, PagePtr *page) const;

 private:
  const unsigned char *buffer_;
  const size_t len_;
};

MemoryPagedByteArray::MemoryPagedByteArray(const unsigned char *buffer,
                                           const size_t len)
    : buffer_(buffer), len_(len) {}

size_t MemoryPagedByteArray::length() const { return len_; }

size_t MemoryPagedByteArray::pageSize() const { return len_; }

void MemoryPagedByteArray::getPage(size_t /* page_index */,
                                   const unsigned char **begin,
                                   const unsigned char **end,
                                   PagePtr *page) const {
  *begin = buffer_;
  *end = buffer_ + len_;
  *page = PagePtr();
}

// A functor that does nothing. This is used as a no-op shared pointer
// deallocator below.
class NullFunctor {
 public:
  void operator()() {}
  void operator()(PagedByteArray * /* p */) const {}
};
}  // namespace

PagedByteArray::~PagedByteArray() {}

RangeCheckedBytePtr::RangeCheckedBytePtr()
    : array_(),
      page_data_(NULL),
      current_pos_(0),
      sub_array_begin_(0),
      sub_array_end_(0),
      page_begin_offset_(0),
      current_page_len_(0),
      error_flag_(RANGE_CHECKED_BYTE_ERROR) {}

RangeCheckedBytePtr::RangeCheckedBytePtr(const unsigned char *array,
                                         const size_t len)
    : array_(new MemoryPagedByteArray(array, len)),
      page_data_(NULL),
      current_pos_(0),
      sub_array_begin_(0),
      sub_array_end_(len),
      page_begin_offset_(0),
      current_page_len_(0),
      error_flag_(RANGE_CHECKED_BYTE_SUCCESS) {
  assert(array);
  if (array == NULL) {
    error_flag_ = RANGE_CHECKED_BYTE_ERROR;
  }
}

RangeCheckedBytePtr::RangeCheckedBytePtr(PagedByteArray *array)
    : array_(array, NullFunctor()),
      page_data_(NULL),
      current_pos_(0),
      sub_array_begin_(0),
      sub_array_end_(array->length()),
      page_begin_offset_(0),
      current_page_len_(0),
      error_flag_(RANGE_CHECKED_BYTE_SUCCESS) {}

RangeCheckedBytePtr RangeCheckedBytePtr::invalidPointer() {
  return RangeCheckedBytePtr();
}

RangeCheckedBytePtr RangeCheckedBytePtr::pointerToSubArray(
    size_t pos, size_t length) const {
  RangeCheckedBytePtr sub_result = (*this) + pos;
  if (!sub_result.errorOccurred() && length <= sub_result.remainingLength()) {
    sub_result.sub_array_begin_ = sub_result.current_pos_;
    sub_result.sub_array_end_ = sub_result.sub_array_begin_ + length;

    // Restrict the boundaries of the current page to the newly set sub-array.
    sub_result.restrictPageToSubArray();

    return sub_result;
  } else {
    error_flag_ = RANGE_CHECKED_BYTE_ERROR;
    return invalidPointer();
  }
}

size_t RangeCheckedBytePtr::offsetInArray() const {
  // sub_array_begin_ <= current_pos_ is a class invariant, but protect
  // against violations of this invariant.
  if (sub_array_begin_ <= current_pos_) {
    return current_pos_ - sub_array_begin_;
  } else {
    assert(false);
    return 0;
  }
}

std::string RangeCheckedBytePtr::substr(size_t pos, size_t length) const {
  std::vector<unsigned char> bytes = extractBytes(pos, length);
  std::string result;
  result.reserve(bytes.size());
  for (size_t i = 0; i < bytes.size(); ++i) {
    result.push_back(static_cast<char>(bytes[i]));
  }
  return result;
}

std::vector<unsigned char> RangeCheckedBytePtr::extractBytes(
    size_t pos, size_t length) const {
  std::vector<unsigned char> result;
  if (pos + length < pos /* overflow */ || remainingLength() < pos + length) {
    BREAK_IF_DEBUGGING();
    error_flag_ = RANGE_CHECKED_BYTE_ERROR_OVERFLOW;
    return result;
  }
  result.reserve(length);
  for (size_t i = 0; i < length; ++i) {
    result.push_back((*this)[pos + i]);
  }
  return result;
}

bool operator==(const RangeCheckedBytePtr &x, const RangeCheckedBytePtr &y) {
  if (x.array_ != y.array_) {
    assert(false);
    return false;
  }

  return x.current_pos_ == y.current_pos_;
}

bool operator!=(const RangeCheckedBytePtr &x, const RangeCheckedBytePtr &y) {
  return !(x == y);
}

void RangeCheckedBytePtr::loadPageForOffset(size_t offset) const {
  // The offset should always lie within the bounds of the sub-array (this
  // condition is enforced at the callsite). However, even if the offset lies
  // outside the sub-array, the restrictPageToSubArray() call at the end
  // ensures that the object is left in a consistent state that maintains the
  // class invariants.
  assert(offset >= sub_array_begin_ && offset < sub_array_end_);

  // Ensure that offset lies within the array.
  if (offset >= array_->length()) {
    assert(false);
    return;
  }

  // Determine the index of the page to request.
  size_t page_index = offset / array_->pageSize();

  // Get the page.
  const unsigned char *page_begin;
  const unsigned char *page_end;
  array_->getPage(page_index, &page_begin, &page_end, &page_);

  // Ensure that the page has the expected length (as specified in the
  // PagedByteArray interface).
  size_t expected_page_size = array_->pageSize();
  if (page_index == (array_->length() - 1) / array_->pageSize()) {
    expected_page_size = array_->length() - array_->pageSize() * page_index;
  }
  if ((page_end < page_begin) ||
      (static_cast<size_t>(page_end - page_begin) != expected_page_size)) {
    assert(false);
    return;
  }

  // Remember information about page.
  page_data_ = page_begin;
  page_begin_offset_ = page_index * array_->pageSize();
  current_page_len_ = static_cast<size_t>(page_end - page_begin);

  // Restrict the boundaries of the page to lie within the sub-array.
  restrictPageToSubArray();
}

void RangeCheckedBytePtr::restrictPageToSubArray() const {
  // Restrict the current page's boundaries so that it is always contained
  // completely within the extent of the sub-array.
  // This function is purposely designed to work correctly in the following
  // two special cases:
  // a) The current page lies entirely outside the sub-array. In this case,
  //    current_page_len_ will be set to zero. page_data_ may either remain
  //    unchanged or may be changed to point one element beyond the end of the
  //    page, depending on whether the current page lies before or after the
  //    sub-array.
  // b) The current page is in the state as initialized by the constructor
  //    (i.e. page_data_ is NULL and current_page_len_ is zero). In this case,
  //    page_data_ and current_page_len_ will remain unchanged.

  // Does the beginning of the page lie before the beginning of the sub-array?
  if (page_begin_offset_ < sub_array_begin_) {
    // Compute amount by which to shorten page.
    size_t amount_to_shorten = sub_array_begin_ - page_begin_offset_;
    if (amount_to_shorten > current_page_len_) {
      amount_to_shorten = current_page_len_;
    }

    // Adjust beginning of page accordingly.
    page_begin_offset_ += amount_to_shorten;
    page_data_ += amount_to_shorten;
    current_page_len_ -= amount_to_shorten;
  }

  // Does the end of the page lie beyond the end of the sub-array?
  if (page_begin_offset_ + current_page_len_ > sub_array_end_) {
    // Reduce length of page accordingly.
    size_t new_len = sub_array_end_ - page_begin_offset_;
    if (new_len > current_page_len_) {
      new_len = current_page_len_;
    }
    current_page_len_ = new_len;
  }
}

int memcmp(const RangeCheckedBytePtr &x, const RangeCheckedBytePtr &y,
           size_t num) {
  std::vector<unsigned char> x_vec = x.extractBytes(0, num);
  std::vector<unsigned char> y_vec = y.extractBytes(0, num);

  if (!x.errorOccurred() && !y.errorOccurred()) {
    return ::memcmp(&x_vec[0], &y_vec[0], num);
  } else {
    // return an arbitrary value
    return -1;
  }
}

int strcmp(const RangeCheckedBytePtr &x, const std::string &y) {
  std::vector<unsigned char> x_vec = x.extractBytes(0, y.length());

  if (!x.errorOccurred()) {
    return ::memcmp(&x_vec[0], y.c_str(), y.length());
  } else {
    // return an arbitrary value
    return -1;
  }
}

size_t strlen(const RangeCheckedBytePtr &src) {
  size_t len = 0;
  RangeCheckedBytePtr str = src;
  while (!str.errorOccurred() && (str[0] != '\0')) {
    str++;
    len++;
  }
  return len;
}

int16 Get16s(const RangeCheckedBytePtr &input, const bool big_endian,
             MemoryStatus *status) {
  const uint16 unsigned_value = Get16u(input, big_endian, status);
  if (*status != RANGE_CHECKED_BYTE_SUCCESS) {
    // Return an arbitrary value.
    return 0;
  }

  // Convert the two's-complement signed integer encoded in 'unsigned_value'
  // into a signed representation in the implementation's native representation
  // for signed integers. An optimized Blaze build (x64) compiles all of the
  // following code to a no-op (as of this writing).
  // For further details, see the corresponding comment in Get32s().
  if (unsigned_value == 0x8000u) {
    return static_cast<int16>(-0x8000);
  } else if (unsigned_value > 0x8000u) {
    return -static_cast<int16>(0x10000u - unsigned_value);
  } else {
    return static_cast<int16>(unsigned_value);
  }
}

uint16 Get16u(const RangeCheckedBytePtr &input, const bool big_endian,
              MemoryStatus *status) {
  if (input.remainingLength() < 2) {
    if (status && *status == RANGE_CHECKED_BYTE_SUCCESS) {
      *status = RANGE_CHECKED_BYTE_ERROR;
    }
    // Return an arbitrary value.
    return 0;
  }
  if (big_endian) {
    return (static_cast<uint16>(input[0]) << 8) | static_cast<uint16>(input[1]);
  } else {
    return (static_cast<uint16>(input[1]) << 8) | static_cast<uint16>(input[0]);
  }
}

int32 Get32s(const RangeCheckedBytePtr &input, const bool big_endian,
             MemoryStatus *status) {
  const uint32 unsigned_value = Get32u(input, big_endian, status);
  if (*status != RANGE_CHECKED_BYTE_SUCCESS) {
    // Return an arbitrary value.
    return 0;
  }

  // Convert the two's-complement signed integer encoded in 'unsigned_value'
  // into a signed representation in the implementation's native representation
  // for signed integers.
  // For all practical purposes, the same result could be obtained simply by
  // casting unsigned_value to int32; the result of this is
  // implementation-defined, but on all of the platforms we care about, it does
  // what we want.
  // The code below, however, arguably has the aesthetic advantage of being
  // independent of the representation for signed integers chosen by the
  // implementation, as long as 'int' and 'unsigned' have the required range to
  // represent all of the required values.
  // An optimized Blaze build (x64) compiles all of the following code to a
  // no-op (as of this writing); i.e. the value that Get32u() returned in %eax
  // is left unchanged.
  if (unsigned_value == 0x80000000u) {
    // Read here on why the constant expression is written this way:
    // http://stackoverflow.com/questions/14695118
    return -0x7fffffff - 1;
  } else if (unsigned_value > 0x80000000u) {
    // The expression
    //   0xffffffffu - unsigned_value + 1
    // is a portable way of flipping the sign of a twos-complement signed
    // integer whose binary representation is stored in an unsigned integer.
    // '0xffffffffu + 1' is used in preference to simply '0' because it makes
    // it clearer that the correct result will be obtained even if an int is
    // greater than 32 bits. The '0xffffffffu + 1' is "spread out" around
    // 'unsigned_value' to prevent the compiler from warning about an
    // integral constant overflow. ('0' would produce the correct result in
    // this case too but would rely in a more subtle way on the rules for
    // unsigned wraparound.)
    return -static_cast<int32>(0xffffffffu - unsigned_value + 1);
  } else {
    return static_cast<int32>(unsigned_value);
  }
}

uint32 Get32u(const RangeCheckedBytePtr &input, const bool big_endian,
              MemoryStatus *status) {
  if (input.remainingLength() < 4) {
    if (status && *status == RANGE_CHECKED_BYTE_SUCCESS) {
      *status = RANGE_CHECKED_BYTE_ERROR;
    }
    // Return an arbitrary value.
    return 0;
  }
  if (big_endian) {
    return (static_cast<uint32>(input[0]) << 24) |
           (static_cast<uint32>(input[1]) << 16) |
           (static_cast<uint32>(input[2]) << 8) |
           (static_cast<uint32>(input[3]) << 0);
  } else {
    return (static_cast<uint32>(input[3]) << 24) |
           (static_cast<uint32>(input[2]) << 16) |
           (static_cast<uint32>(input[1]) << 8) |
           (static_cast<uint32>(input[0]) << 0);
  }
}

}  // namespace binary_parse
}  // namespace piex
