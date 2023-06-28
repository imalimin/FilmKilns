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
// This file offers functions to determine the type of binary input source. The
// type recognition here is not 100% accurate, it only offers a quick and rough
// check about the input source. The general functions use RangeCheckedBytePtr
// as input, there are also linux only functions that use StringPiece as input.
// A linux only IsRawLite() method is also implemented.
// The "lite" implementation focuses on performance and guarantees to not read
// more than specified by GetNumberOfBytesForIsRawLite.

#ifndef PIEX_IMAGE_TYPE_RECOGNITION_IMAGE_TYPE_RECOGNITION_LITE_H_
#define PIEX_IMAGE_TYPE_RECOGNITION_IMAGE_TYPE_RECOGNITION_LITE_H_

#include <stddef.h>

#include "src/binary_parse/range_checked_byte_ptr.h"

namespace piex {
namespace image_type_recognition {

// Type of RAW images. Keep the order in alphabet.
enum RawImageTypes {
  // Non-RAW-image type
  kNonRawImage = 0,

  // raw image types
  kArwImage,
  kCr2Image,
  kCrwImage,
  kDcrImage,
  kDngImage,
  kKdcImage,
  kMosImage,
  kMrwImage,
  kNefImage,
  kNrwImage,
  kOrfImage,
  kPefImage,
  kQtkImage,
  kRafImage,
  kRawContaxNImage,
  kRw2Image,
  kSrwImage,
  kX3fImage,
};

// Checks if the given type is a RAW image type.
bool IsRaw(const RawImageTypes type);

// Checks if the given source is from given type.
bool IsOfType(const binary_parse::RangeCheckedBytePtr& source,
              const RawImageTypes type);

// This function will check the source and return the corresponding image type.
// If the source is not a recognizable type, this function will return
// kNonRawImage.
RawImageTypes RecognizeRawImageTypeLite(
    const binary_parse::RangeCheckedBytePtr& source);

// Returns the maximum number of bytes needed to recognize a RAW image type in
// IsRawLite().
size_t GetNumberOfBytesForIsRawLite();

// Returns the maximum number of bytes needed to recognize a RAF image type in
// IsOfType().
size_t GetNumberOfBytesForIsOfType(const RawImageTypes type);

// This function will check if the source belongs to one of the known RAW types.
bool IsRawLite(const binary_parse::RangeCheckedBytePtr& source);

}  // namespace image_type_recognition
}  // namespace piex

#endif  // PIEX_IMAGE_TYPE_RECOGNITION_IMAGE_TYPE_RECOGNITION_LITE_H_
