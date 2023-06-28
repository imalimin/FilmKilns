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
// The purpose of the preview-image-extractor (piex) is to find and extract the
// largest JPEG compressed preview image contained in a RAW file.
//
// Even for unsupported RAW files we want to provide high quality images using a
// dedicated, small and portable library. That is possible by taking the preview
// image contained in all RAW files.
//
// Typically a preview image is stored as JPEG compressed, full size (or at
// least half size) image in a RAW file.
//
// A typical client code snippet:
//
//   // In C++
//   PreviewImageData image_data;
//   unique_ptr<StreamInterface> data_stream(new DataStream(file));
//   Error err = GetPreviewImageData(data_stream.get(), &image_data));
//   if (err == Error::kFail) {
//     // The input data seems to be broken.
//     return;
//   } else if (err == Error::kUnsupported) {
//     // The input data is not supported.
//     return;
//   }
//
//   // Uncompress the JPEG as usual, e.g. on Android with the BitmapFactory:
//   // In Java
//   Bitmap bitmap = BitmapFactory.decodeByteArray(
//       file.at(image_data.preview_offset), image_data.preview_length);

#ifndef PIEX_PIEX_H_
#define PIEX_PIEX_H_

#include <string>
#include <vector>

#include "src/piex_types.h"

namespace piex {

// Returns the maximum number of bytes IsRaw() will read from the stream.
size_t BytesRequiredForIsRaw();

// Returns true if 'data' contains a RAW file format, even if it is not
// supported by Piex, false otherwise. Reads at most BytesRequiredForIsRaw()
// from the stream.
bool IsRaw(StreamInterface* data);

// Gets the largest JPEG compressed preview image data. On success
// 'preview_image_data' contains image metadata, the unverified length and the
// offset to a JPEG compressed image from the beginning of the file.
//
// Returns 'kFail' when something with the data is wrong.
// Returns 'kUnsupported' if file format is not supported.
//
// One could check the "preview_image_data->preview_length != 0" for the
// existance of a preview image.
Error GetPreviewImageData(StreamInterface* data,
                          PreviewImageData* preview_image_data);

// Returns true if the full width and height and the mosaic pattern dimension of
// a DNG image could be obtained. False otherwise.
bool GetDngInformation(StreamInterface* data, std::uint32_t* width,
                       std::uint32_t* height,
                       std::vector<std::uint32_t>* cfa_pattern_dim);

// Returns true if Exif orientation for the image can be obtained. False
// otherwise.
bool GetOrientation(StreamInterface* data, std::uint32_t* orientation);

// Returns a vector of upper case file extensions, which are used as a first
// step to quickly guess a supported file format.
std::vector<std::string> SupportedExtensions();

}  // namespace piex

#endif  // PIEX_PIEX_H_
