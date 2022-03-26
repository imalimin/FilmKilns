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
//
// This file implements the image type recognition algorithm. Functions, which
// will check each single image type, are implemented based on the comparisons
// of magic numbers or signature strings. Other checks (e.g endianness, general
// tiff magic number "42", etc.) could also be used in some of those functions
// to make the type recognition more stable. Those checks are designed
// according to the format spcifications and our own experiments. Notice that
// the magic numbers and signature strings may have different binary values
// according to different endiannesses.
#include "src/image_type_recognition/image_type_recognition_lite.h"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

#include "src/binary_parse/range_checked_byte_ptr.h"

namespace piex {
namespace image_type_recognition {
namespace {

using std::string;
using binary_parse::MemoryStatus;
using binary_parse::RangeCheckedBytePtr;

// Base class for checking image type. For each image type, one should create an
// inherited class and do the implementation.
class TypeChecker {
 public:
  // Comparing function, whihc is used for sorting.
  static bool Compare(const TypeChecker* a, const TypeChecker* b) {
    assert(a);
    assert(b);
    return a->RequestedSize() < b->RequestedSize();
  }

  virtual ~TypeChecker() {}

  // Returns the type of current checker.
  virtual RawImageTypes Type() const = 0;

  // Returns the requested data size (in bytes) for current checker. The checker
  // guarantees that it will not read more than this size.
  virtual size_t RequestedSize() const = 0;

  // Checks if source data belongs to current checker type.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const = 0;

 protected:
  // Limits the source length to the RequestedSize(), using it guarantees that
  // we will not read more than this size from the source.
  RangeCheckedBytePtr LimitSource(const RangeCheckedBytePtr& source) const {
    return source.pointerToSubArray(0 /* pos */, RequestedSize());
  }
};

// Check if the uint16 value at (source + offset) is equal to the target value.
bool CheckUInt16Value(const RangeCheckedBytePtr& source,
                      const size_t source_offset, const bool use_big_endian,
                      const unsigned short target_value) {  // NOLINT
  MemoryStatus status = binary_parse::RANGE_CHECKED_BYTE_SUCCESS;
  const unsigned short value = binary_parse::Get16u(  // NOLINT
      source + source_offset, use_big_endian, &status);
  if (status != binary_parse::RANGE_CHECKED_BYTE_SUCCESS) {
    return false;
  }
  return (target_value == value);
}

// Check if the uint32 value at (source + offset) is equal to the target value.
bool CheckUInt32Value(const RangeCheckedBytePtr& source,
                      const size_t source_offset, const bool use_big_endian,
                      const unsigned int target_value) {
  MemoryStatus status = binary_parse::RANGE_CHECKED_BYTE_SUCCESS;
  const unsigned int value =
      binary_parse::Get32u(source + source_offset, use_big_endian, &status);
  if (status != binary_parse::RANGE_CHECKED_BYTE_SUCCESS) {
    return false;
  }
  return (target_value == value);
}

// Determine the endianness. The return value is NOT the endianness indicator,
// it's just that this function was successful.
bool DetermineEndianness(const RangeCheckedBytePtr& source,
                         bool* is_big_endian) {
  if (source.remainingLength() < 2) {
    return false;
  }

  if (source[0] == 0x49 && source[1] == 0x49) {
    *is_big_endian = false;
  } else if (source[0] == 0x4D && source[1] == 0x4D) {
    *is_big_endian = true;
  } else {
    return false;
  }
  return true;
}

// Check if signature string can match to the same length string start from
// (source + offset). The signature string will be used as longer magic number
// series.
bool IsSignatureMatched(const RangeCheckedBytePtr& source,
                        const size_t source_offset, const string& signature) {
  return source.substr(source_offset, signature.size()) == signature;
}

// Check if signature is found in [source + offset, source + offset + range].
bool IsSignatureFound(const RangeCheckedBytePtr& source,
                      const size_t search_offset, const size_t search_range,
                      const string& signature, size_t* first_matched) {
  if (source.remainingLength() < search_offset + search_range) {
    return false;
  }

  // The index must be in range [offset, offset + range - sizeof(signature)], so
  // that it can guarantee that it will not read outside of range.
  for (size_t i = search_offset;
       i < search_offset + search_range - signature.size(); ++i) {
    if (IsSignatureMatched(source, i, signature)) {
      if (first_matched) {
        *first_matched = i;
      }
      return true;
    }
  }
  return false;
}

// Sony RAW format.
class ArwTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kArwImage; }

  virtual size_t RequestedSize() const { return 5000; }

  // Check multiple points:
  // 1. valid endianness at the beginning of the file;
  // 2. correct tiff magic number at the (offset == 8) position of the file;
  // 3. signature "SONY" in first requested bytes;
  // 4. correct signature for (section + version) in first requested bytes.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(limited_source, &use_big_endian)) {
      return false;
    }

    const unsigned short kTiffMagic = 0x2A;  // NOLINT
    const unsigned int kTiffOffset = 8;
    if (!CheckUInt16Value(limited_source, 2 /* offset */, use_big_endian,
                          kTiffMagic) ||
        !CheckUInt32Value(limited_source, 4 /* offset */, use_big_endian,
                          kTiffOffset)) {
      return false;
    }

    // Search for kSignatureSony in first requested bytes
    const string kSignatureSony("SONY");
    if (!IsSignatureFound(limited_source, 0 /* offset */, RequestedSize(),
                          kSignatureSony, NULL)) {
      return false;
    }

    // Search for (kSignatureFileTypeSection + kSignatureVersions[i]) in first
    // requested bytes
    const string kSignatureSection("\x00\xb0\x01\x00\x04\x00\x00\x00", 8);
    const int kSignatureVersionsSize = 5;
    const string kSignatureVersions[kSignatureVersionsSize] = {
        string("\x02\x00", 2),  // ARW 1.0
        string("\x03\x00", 2),  // ARW 2.0
        string("\x03\x01", 2),  // ARW 2.1
        string("\x03\x02", 2),  // ARW 2.2
        string("\x03\x03", 2),  // ARW 2.3
    };
    bool matched = false;
    for (int i = 0; i < kSignatureVersionsSize; ++i) {
      matched = matched || IsSignatureFound(
                               limited_source, 0 /* offset */, RequestedSize(),
                               kSignatureSection + kSignatureVersions[i], NULL);
    }
    return matched;
  }
};

// Canon RAW (CR2 extension).
class Cr2TypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kCr2Image; }

  virtual size_t RequestedSize() const { return 16; }

  // Check multiple points:
  // 1. valid endianness at the beginning of the file;
  // 2. magic number "42" at the (offset == 2) position of the file;
  // 3. signature "CR2" at the (offset == 8) position of the file.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(limited_source, &use_big_endian)) {
      return false;
    }

    const unsigned short kTag = 42;  // NOLINT
    if (!CheckUInt16Value(limited_source, 2 /* offset */, use_big_endian,
                          kTag)) {
      return false;
    }

    const string kSignature("CR\2\0", 4);
    return IsSignatureMatched(limited_source, 8 /* offset */, kSignature);
  }
};

// Canon RAW (CRW extension).
class CrwTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kCrwImage; }

  virtual size_t RequestedSize() const { return 14; }

  // Check only the signature at the (offset == 6) position of the file.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(limited_source, &use_big_endian)) {
      return false;
    }

    string signature;
    if (use_big_endian) {
      signature = string("\x00\x10\xba\xb0\xac\xbb\x00\x02", 8);
    } else {
      signature = string("HEAPCCDR");
    }
    return IsSignatureMatched(limited_source, 6 /* offset */, signature);
  }
};

// Kodak RAW.
class DcrTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kDcrImage; }

  virtual size_t RequestedSize() const { return 5000; }

  // Check two different cases, only need to fulfill one of the two:
  // 1. signature at the (offset == 16) position of the file;
  // 2. two tags (OriginalFileName and FirmwareVersion) can be found in the
  // first requested bytes of the file.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(limited_source, &use_big_endian)) {
      return false;
    }

    // Case 1: has signature
    const string kSignature(
        "\x4b\x4f\x44\x41\x4b\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20", 16);
    if (IsSignatureMatched(limited_source, 16 /* offset */, kSignature)) {
      return true;
    }

    // Case 2: search for tags in first requested bytes
    string kIfdTags[2];
    if (use_big_endian) {
      kIfdTags[0] = string("\x03\xe9\x00\x02", 4);  // OriginalFileName
      kIfdTags[1] = string("\x0c\xe5\x00\x02", 4);  // FirmwareVersion
    } else {
      kIfdTags[0] = string("\xe9\x03\x02\x00", 4);  // OriginalFileName
      kIfdTags[1] = string("\xe5\x0c\x02\x00", 4);  // FirmwareVersion
    }
    return IsSignatureFound(limited_source, 0 /* offset */, RequestedSize(),
                            kIfdTags[0], NULL) &&
           IsSignatureFound(limited_source, 0 /* offset */, RequestedSize(),
                            kIfdTags[1], NULL);
  }
};

// Digital Negative RAW.
class DngTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kDngImage; }

  virtual size_t RequestedSize() const { return 1024; }

  // Check multiple points:
  // 1. valid endianness at the beginning of the file;
  // 2. at least two dng specific tags in the first requested bytes of the
  // file
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(limited_source, &use_big_endian)) {
      return false;
    }

    // Search tags in first requested bytes and verify the order of them.
    const int kTagsCount = 5;
    string dng_tags[kTagsCount];
    if (use_big_endian) {
      dng_tags[0] =
          string("\xc6\x12\x00\x01\x00\x00\x00\x04", 8);  // tag: 50706
      dng_tags[1] =
          string("\xc6\x13\x00\x01\x00\x00\x00\x04", 8);  // tag: 50707
      dng_tags[2] = string("\xc6\x14\x00\x02", 4);        // tag: 50708
      dng_tags[3] = string("\xc6\x20", 2);                // tag: 50720
      dng_tags[4] =
          string("\xc6\x2d\x00\x04\x00\x00\x00\x01", 8);  // tag: 50733
    } else {
      dng_tags[0] =
          string("\x12\xc6\x01\x00\x04\x00\x00\x00", 8);  // tag: 50706
      dng_tags[1] =
          string("\x13\xc6\x01\x00\x04\x00\x00\x00", 8);  // tag: 50707
      dng_tags[2] = string("\x14\xc6\x02\x00", 4);        // tag: 50708
      dng_tags[3] = string("\x20\xc6", 2);                // tag: 50720
      dng_tags[4] =
          string("\x2d\xc6\x04\x00\x01\x00\x00\x00", 8);  // tag: 50733
    }
    int tags_found = 0;
    for (int i = 0; i < kTagsCount; ++i) {
      if (IsSignatureFound(limited_source, 0 /* offset */, RequestedSize(),
                           dng_tags[i], NULL)) {
        tags_found++;
      }
    }
    return tags_found >= 2;
  }
};

// Kodak RAW.
class KdcTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kKdcImage; }

  virtual size_t RequestedSize() const { return 5000; }

  // Check two points:
  // 1. valid endianness at the beginning of the file;
  // 2. two tags (WhiteBalance and SerialNumber) in the first requested bytes.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(limited_source, &use_big_endian)) {
      return false;
    }

    // Search in first requested bytes
    const size_t kIfdTagsSize = 2;
    string kIfdTags[kIfdTagsSize];
    if (use_big_endian) {
      kIfdTags[0] = string("\xfa\x0d\x00\x01", 4);  // WhiteBalance
      kIfdTags[1] = string("\xfa\x00\x00\x02", 4);  // SerialNumber
    } else {
      kIfdTags[0] = string("\x0d\xfa\x01\x00", 4);  // WhiteBalance
      kIfdTags[1] = string("\x00\xfa\x02\x00", 4);  // SerialNumber
    }

    return IsSignatureFound(limited_source, 0 /* offset */, RequestedSize(),
                            kIfdTags[0], NULL) &&
           IsSignatureFound(limited_source, 0 /* offset */, RequestedSize(),
                            kIfdTags[1], NULL);
  }
};

// Leaf RAW.
class MosTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kMosImage; }

  virtual size_t RequestedSize() const { return 5000; }

  // Check two points:
  // 1. valid endianness at the beginning of the file;
  // 2. signature "PKTS    " in the first requested bytes. Note the
  // "whitespace". It's important as they are special binary values.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(source, &use_big_endian)) {
      return false;
    }

    // Search kSignaturePKTS in first requested bytes
    const string kSignaturePKTS("PKTS\x00\x00\x00\x001", 8);
    return IsSignatureFound(limited_source, 0 /* offset */, RequestedSize(),
                            kSignaturePKTS, NULL);
  }
};

// Minolta RAW.
class MrwTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kMrwImage; }

  virtual size_t RequestedSize() const { return 4; }

  // Check only the signature at the beginning of the file.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    // Limits the source length to the RequestedSize(), using it guarantees that
    // we will not read more than this size from the source.
    RangeCheckedBytePtr limited_source =
        source.pointerToSubArray(0 /* pos */, RequestedSize());

    const string kSignature("\0MRM", 4);
    return IsSignatureMatched(limited_source, 0 /* offset */, kSignature);
  }
};

// Check if the file contains a NRW signature "NRW   " in the first requested
// bytes. Note the "whitespace". It's important as they are special binary
// values.
const size_t kRequestedSizeForNrwSignature = 4000;
bool ContainsNrwSignature(const RangeCheckedBytePtr& source) {
  // Search for kSignatureNrw.
  const string kSignatureNrw("NRW\x20\x20\x20", 6);
  return IsSignatureFound(source, 0 /* offset */, kRequestedSizeForNrwSignature,
                          kSignatureNrw, NULL);
}

// Checks if the file contains the signatures for Nikon formats:
// * the general Nikon singature "NIKON" string.
// * the ReferenceBlackWhite tag.
const size_t kRequestedSizeForNikonSignatures = 4000;
bool ContainsNikonSignatures(const RangeCheckedBytePtr& source,
                             const bool use_big_endian) {
  const string kSignatureNikon("NIKON");
  const string kReferenceBlackWhiteTag = use_big_endian
                                             ? string("\x02\x14\x00\x05", 4)
                                             : string("\x14\x02\x05\x00", 4);
  const std::vector<string> kSignatures = {kSignatureNikon,
                                           kReferenceBlackWhiteTag};
  for (auto const& signature : kSignatures) {
    if (!IsSignatureFound(source, 0, kRequestedSizeForNikonSignatures,
                          signature, NULL)) {
      return false;
    }
  }
  return true;
}

// Nikon RAW (NEF extension).
class NefTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kNefImage; }

  virtual size_t RequestedSize() const {
    return std::max(kRequestedSizeForNikonSignatures,
                    kRequestedSizeForNrwSignature);
  }

  // Check multiple points:
  // 1. valid endianness at the beginning of the file;
  // 2. magic number at the (offset == 2) position of the file;
  // 3. the signature "NIKON" in the requested bytes of the file;
  // 4. the ReferenceBlackWhite tag in the requested bytes of the file;
  // 5. does not contain the NRW signature. We may also check a special
  // signature "RAW   " similar to the NRW case, but we got issues in some
  // special images that the signature locates in the middle of the file, and it
  // costs too  long time to check;
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(limited_source, &use_big_endian)) {
      return false;
    }

    const unsigned short kTiffMagic = 0x2A;  // NOLINT
    if (!CheckUInt16Value(limited_source, 2 /* offset */, use_big_endian,
                          kTiffMagic)) {
      return false;
    }

    return ContainsNikonSignatures(limited_source, use_big_endian) &&
           !ContainsNrwSignature(limited_source);  // not NRW
  }
};

// Nikon RAW (NRW extension).
class NrwTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kNrwImage; }

  virtual size_t RequestedSize() const {
    return std::max(kRequestedSizeForNikonSignatures,
                    kRequestedSizeForNrwSignature);
  }

  // Check multiple points:
  // 1. valid endianness at the beginning of the file;
  // 2. magic numbers at the (offset == 2 and offset == 4) positions of the
  // file;
  // 3. the signature "NIKON" in the first requested bytes of the file;
  // 4. the ReferenceBlackWhite tag in the requested bytes of the file;
  // 5. contains the NRW signature;
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(limited_source, &use_big_endian)) {
      return false;
    }

    const unsigned short kTiffMagic = 0x2A;  // NOLINT
    const unsigned int kTiffOffset = 8;
    if (!CheckUInt16Value(limited_source, 2 /* offset */, use_big_endian,
                          kTiffMagic) ||
        !CheckUInt32Value(limited_source, 4 /* offset */, use_big_endian,
                          kTiffOffset)) {
      return false;
    }

    return ContainsNikonSignatures(limited_source, use_big_endian) &&
           ContainsNrwSignature(limited_source);
  }
};

// Olympus RAW.
class OrfTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kOrfImage; }

  virtual size_t RequestedSize() const { return 3000; }

  // Check multiple points:
  // 1. valid endianness at the beginning of the file;
  // 2. tag at the (offset == 2) position of the file;
  // 3. signature "OLYMP" in the first requested bytes.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(limited_source, &use_big_endian)) {
      return false;
    }

    const size_t kTagSize = 2;
    const unsigned short kTag[kTagSize] = {0x4F52, 0x5352};  // NOLINT
    if (!(CheckUInt16Value(limited_source, 2 /* offset */, use_big_endian,
                           kTag[0]) ||
          CheckUInt16Value(limited_source, 2 /* offset */, use_big_endian,
                           kTag[1]))) {
      return false;
    }

    // Search for kSignatureOlymp in first requested bytes
    const string kSignatureOlymp("OLYMP");
    return IsSignatureFound(limited_source, 0 /* offset */, RequestedSize(),
                            kSignatureOlymp, NULL);
  }
};

// Pentax RAW.
class PefTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kPefImage; }

  virtual size_t RequestedSize() const { return 1280; }

  // Check multiple points:
  // 1. valid big endianness at the beginning of the file;
  // 2. magic numbers at the (offset == 2 and offset==4) positions of the file;
  // 3. signature "AOC   " or "PENTAX  " in first requested bytes.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(limited_source, &use_big_endian)) {
      return false;
    }

    const unsigned short kTiffMagic = 0x2A;  // NOLINT
    const unsigned int kTiffOffset = 8;
    if (!CheckUInt16Value(limited_source, 2 /* offset */, use_big_endian,
                          kTiffMagic) ||
        !CheckUInt32Value(limited_source, 4 /* offset */, use_big_endian,
                          kTiffOffset)) {
      return false;
    }

    // Search for kSignatureAOC or kSignaturePENTAX in first requested bytes
    const string kSignatureAOC("\x41\x4f\x43\x00\x4d\x4d", 6);
    const string kSignaturePENTAX("\x50\x45\x4e\x54\x41\x58\x20\x00", 8);
    return IsSignatureFound(limited_source, 0 /* offset */, RequestedSize(),
                            kSignatureAOC, NULL) ||
           IsSignatureFound(limited_source, 0 /* offset */, RequestedSize(),
                            kSignaturePENTAX, NULL);
  }
};

// Apple format.
class QtkTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kQtkImage; }

  virtual size_t RequestedSize() const { return 8; }

  // Check only the signature at the beginning of the file.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    const size_t kSignatureSize = 2;
    const string kSignature[kSignatureSize] = {
        string("qktk\x00\x00\x00\x08", 8), string("qktn\x00\x00\x00\x08", 8),
    };
    return IsSignatureMatched(limited_source, 0 /* offset */, kSignature[0]) ||
           IsSignatureMatched(limited_source, 0 /* offset */, kSignature[1]);
  }
};

// Fuji RAW.
class RafTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kRafImage; }

  virtual size_t RequestedSize() const { return 8; }

  // Check only the signature at the beginning of the file.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    const string kSignature("FUJIFILM");
    return IsSignatureMatched(limited_source, 0 /* offset */, kSignature);
  }
};

// Contax N RAW.
class RawContaxNTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kRawContaxNImage; }

  virtual size_t RequestedSize() const { return 36; }

  // Check only the signature at the (offset == 25) position of the
  // file.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    const string kSignature("ARECOYK");
    return IsSignatureMatched(limited_source, 25, kSignature);
  }
};

// Panasonic RAW.
class Rw2TypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kRw2Image; }

  virtual size_t RequestedSize() const { return 4; }

  // Check two points: 1. valid endianness at the beginning of the
  // file; 2. tag at the (offset == 2) position of the file.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(source, &use_big_endian)) {
      return false;
    }

    const unsigned short kTag = 0x55;  // NOLINT
    return CheckUInt16Value(limited_source, 2 /* offset */, use_big_endian,
                            kTag);
  }
};

// Samsung RAW.
class SrwTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kSrwImage; }

  virtual size_t RequestedSize() const { return 256; }

  // Check multiple points:
  // 1. valid big endianness at the beginning of the file;
  // 2. magic numbers at the (offset == 2 and offset==4) positions of the file;
  // 3. the signature "SAMSUNG" in the requested bytes of the file;
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    bool use_big_endian;
    if (!DetermineEndianness(source, &use_big_endian)) {
      return false;
    }

    const unsigned short kTiffMagic = 0x2A;  // NOLINT
    const unsigned int kTiffOffset = 8;
    if (!CheckUInt16Value(limited_source, 2 /* offset */, use_big_endian,
                          kTiffMagic) ||
        !CheckUInt32Value(limited_source, 4 /* offset */, use_big_endian,
                          kTiffOffset)) {
      return false;
    }

    const string kSignature("SAMSUNG");
    if (!IsSignatureFound(source, 0, RequestedSize(), kSignature, NULL)) {
      return false;
    }
    return true;
  }
};

// Sigma / Polaroid RAW.
class X3fTypeChecker : public TypeChecker {
 public:
  virtual RawImageTypes Type() const { return kX3fImage; }

  virtual size_t RequestedSize() const { return 4; }

  // Check only the signature at the beginning of the file.
  virtual bool IsMyType(const RangeCheckedBytePtr& source) const {
    RangeCheckedBytePtr limited_source = LimitSource(source);

    const string kSignature("FOVb", 4);
    return IsSignatureMatched(limited_source, 0 /* offset */, kSignature);
  }
};

// This class contains the list of all type checkers. One should used this list
// as a whole to execute the image type recognition.
class TypeCheckerList {
 public:
  TypeCheckerList() {
    // Add all supported RAW type checkers here.
    checkers_.push_back(new ArwTypeChecker());
    checkers_.push_back(new Cr2TypeChecker());
    checkers_.push_back(new CrwTypeChecker());
    checkers_.push_back(new DcrTypeChecker());
    checkers_.push_back(new DngTypeChecker());
    checkers_.push_back(new KdcTypeChecker());
    checkers_.push_back(new MosTypeChecker());
    checkers_.push_back(new MrwTypeChecker());
    checkers_.push_back(new NefTypeChecker());
    checkers_.push_back(new NrwTypeChecker());
    checkers_.push_back(new OrfTypeChecker());
    checkers_.push_back(new PefTypeChecker());
    checkers_.push_back(new QtkTypeChecker());
    checkers_.push_back(new RafTypeChecker());
    checkers_.push_back(new RawContaxNTypeChecker());
    checkers_.push_back(new Rw2TypeChecker());
    checkers_.push_back(new SrwTypeChecker());
    checkers_.push_back(new X3fTypeChecker());

    // Sort the checkers by the ascending RequestedSize() to get better
    // performance when checking type.
    std::sort(checkers_.begin(), checkers_.end(), TypeChecker::Compare);
  }

  ~TypeCheckerList() {
    for (size_t i = 0; i < checkers_.size(); ++i) {
      delete checkers_[i];
      checkers_[i] = NULL;
    }
  }

  // Returns the type of source data. If it can not be identified, returns
  // kNonRawImage.
  RawImageTypes GetType(const RangeCheckedBytePtr& source) const {
    for (size_t i = 0; i < checkers_.size(); ++i) {
      if (checkers_[i]->IsMyType(source)) {
        return checkers_[i]->Type();
      }
    }
    return kNonRawImage;
  }

  // Returns the maximum size of requested size of data for identifying image
  // type using this class. The class guarantees that it will not read more than
  // this size.
  size_t RequestedSize() const {
    assert(!checkers_.empty());
    // The checkers_ is ascending sorted. The last element is the maximum.
    return checkers_.back()->RequestedSize();
  }

  bool IsOfType(const RangeCheckedBytePtr& source, const RawImageTypes type) {
    const TypeChecker* type_checker = GetTypeCheckerForType(type);
    if (type_checker) {
      return type_checker->IsMyType(source);
    } else {
      return false;
    }
  }

  size_t RequestedSizeForType(const RawImageTypes type) {
    const TypeChecker* type_checker = GetTypeCheckerForType(type);
    if (type_checker) {
      return type_checker->RequestedSize();
    } else {
      return 0;
    }
  }

 private:
  const TypeChecker* GetTypeCheckerForType(const RawImageTypes type) {
    for (const auto* type_checker : checkers_) {
      if (type_checker->Type() == type) {
        return type_checker;
      }
    }
    return nullptr;
  }

  std::vector<TypeChecker*> checkers_;
};

}  // namespace

bool IsRaw(const RawImageTypes type) {
  switch (type) {
    // Non-RAW-image type
    case kNonRawImage: {
      return false;
    }

    // Raw image types
    case kArwImage:
    case kCr2Image:
    case kCrwImage:
    case kDcrImage:
    case kDngImage:
    case kKdcImage:
    case kMosImage:
    case kMrwImage:
    case kNefImage:
    case kNrwImage:
    case kOrfImage:
    case kPefImage:
    case kQtkImage:
    case kRafImage:
    case kRawContaxNImage:
    case kRw2Image:
    case kSrwImage:
    case kX3fImage: {
      return true;
    }

    default: {
      // Unsupported type!
      assert(false);
    }
  }
  return false;
}

bool IsOfType(const RangeCheckedBytePtr& source, const RawImageTypes type) {
  return TypeCheckerList().IsOfType(source, type);
}

RawImageTypes RecognizeRawImageTypeLite(const RangeCheckedBytePtr& source) {
  return TypeCheckerList().GetType(source);
}

size_t GetNumberOfBytesForIsRawLite() {
  return TypeCheckerList().RequestedSize();
}

size_t GetNumberOfBytesForIsOfType(const RawImageTypes type) {
  return TypeCheckerList().RequestedSizeForType(type);
}

bool IsRawLite(const RangeCheckedBytePtr& source) {
  return IsRaw(RecognizeRawImageTypeLite(source));
}

}  // namespace image_type_recognition
}  // namespace piex
