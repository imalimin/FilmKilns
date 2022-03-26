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

#include "src/piex.h"

#include <cstdint>
#include <limits>
#include <set>
#include <vector>

#include "src/binary_parse/range_checked_byte_ptr.h"
#include "src/image_type_recognition/image_type_recognition_lite.h"
#include "src/tiff_parser.h"

namespace piex {
namespace {

using binary_parse::RangeCheckedBytePtr;
using image_type_recognition::RawImageTypes;
using image_type_recognition::RecognizeRawImageTypeLite;
using tiff_directory::Endian;
using tiff_directory::TiffDirectory;

const std::uint32_t kRafOffsetToPreviewOffset = 84;

bool GetDngInformation(const tiff_directory::TiffDirectory& tiff_directory,
                       std::uint32_t* width, std::uint32_t* height,
                       std::vector<std::uint32_t>* cfa_pattern_dim) {
  if (!GetFullDimension32(tiff_directory, width, height) || *width == 0 ||
      *height == 0) {
    return false;
  }

  if (!tiff_directory.Get(kTiffTagCfaPatternDim, cfa_pattern_dim) ||
      cfa_pattern_dim->size() != 2) {
    return false;
  }
  return true;
}

bool GetDngInformation(const TagSet& extended_tags, StreamInterface* data,
                       std::uint32_t* width, std::uint32_t* height,
                       std::vector<std::uint32_t>* cfa_pattern_dim) {
  TagSet desired_tags = {kExifTagDefaultCropSize, kTiffTagCfaPatternDim,
                         kTiffTagExifIfd, kTiffTagSubFileType};
  desired_tags.insert(extended_tags.cbegin(), extended_tags.cend());

  TiffParser tiff_parser(data, 0 /* offset */);

  TiffContent tiff_content;
  if (!tiff_parser.Parse(desired_tags, 1, &tiff_content) ||
      tiff_content.tiff_directory.empty()) {
    return false;
  }

  // If IFD0 contains already the full dimensions we do not parse into the sub
  // IFD.
  const TiffDirectory& tiff_directory = tiff_content.tiff_directory[0];
  if (tiff_directory.GetSubDirectories().empty()) {
    return GetDngInformation(tiff_directory, width, height, cfa_pattern_dim);
  } else {
    return GetDngInformation(tiff_directory.GetSubDirectories()[0], width,
                             height, cfa_pattern_dim);
  }
}

bool GetPreviewData(const TagSet& extended_tags,
                    const std::uint32_t tiff_offset,
                    const std::uint32_t number_of_ifds, StreamInterface* stream,
                    TiffContent* tiff_content,
                    PreviewImageData* preview_image_data) {
  TagSet desired_tags = {
      kExifTagColorSpace, kExifTagDateTimeOriginal, kExifTagExposureTime,
      kExifTagFnumber,    kExifTagFocalLength,      kExifTagGps,
      kExifTagIsoSpeed,   kTiffTagCompression,      kTiffTagDateTime,
      kTiffTagExifIfd,    kTiffTagCfaPatternDim,    kTiffTagMake,
      kTiffTagModel,      kTiffTagOrientation,      kTiffTagPhotometric};
  desired_tags.insert(extended_tags.cbegin(), extended_tags.cend());

  TiffParser tiff_parser(stream, tiff_offset);

  if (!tiff_parser.Parse(desired_tags, number_of_ifds, tiff_content)) {
    return false;
  }
  if (tiff_content->tiff_directory.empty()) {
    // Returns false if the stream does not contain any TIFF structure.
    return false;
  }
  return tiff_parser.GetPreviewImageData(*tiff_content, preview_image_data);
}

bool GetPreviewData(const TagSet& extended_tags,
                    const std::uint32_t number_of_ifds, StreamInterface* stream,
                    PreviewImageData* preview_image_data) {
  const std::uint32_t kTiffOffset = 0;
  TiffContent tiff_content;
  return GetPreviewData(extended_tags, kTiffOffset, number_of_ifds, stream,
                        &tiff_content, preview_image_data);
}

bool GetExifData(const std::uint32_t exif_offset, StreamInterface* stream,
                 PreviewImageData* preview_image_data) {
  const TagSet kExtendedTags = {kTiffTagJpegByteCount, kTiffTagJpegOffset};
  const std::uint32_t kNumberOfIfds = 2;
  TiffContent tiff_content;
  return GetPreviewData(kExtendedTags, exif_offset, kNumberOfIfds, stream,
                        &tiff_content, preview_image_data);
}

// Reads the jpeg compressed thumbnail information.
void GetThumbnailOffsetAndLength(const TagSet& extended_tags,
                                 StreamInterface* stream,
                                 PreviewImageData* preview_image_data) {
  TagSet desired_tags = {kTiffTagJpegByteCount, kTiffTagJpegOffset};
  desired_tags.insert(extended_tags.cbegin(), extended_tags.cend());

  const std::uint32_t kNumberOfIfds = 2;
  PreviewImageData thumbnail_data;
  if (GetPreviewData(desired_tags, kNumberOfIfds, stream, &thumbnail_data)) {
    preview_image_data->thumbnail = thumbnail_data.thumbnail;
  }
}

bool GetExifIfd(const Endian endian, StreamInterface* stream,
                TiffDirectory* exif_ifd) {
  const std::uint32_t kTiffOffset = 0;
  std::uint32_t offset_to_ifd;
  if (!Get32u(stream, sizeof(offset_to_ifd), endian, &offset_to_ifd)) {
    return false;
  }

  std::uint32_t next_ifd_offset;
  TiffDirectory tiff_ifd(endian);
  if (!ParseDirectory(kTiffOffset, offset_to_ifd, endian, {kTiffTagExifIfd},
                      stream, &tiff_ifd, &next_ifd_offset)) {
    return false;
  }

  std::uint32_t exif_offset;
  if (tiff_ifd.Get(kTiffTagExifIfd, &exif_offset)) {
    return ParseDirectory(kTiffOffset, exif_offset, endian,
                          {kExifTagMakernotes}, stream, exif_ifd,
                          &next_ifd_offset);
  }

  return true;
}

bool GetMakernoteIfd(const TiffDirectory& exif_ifd, const Endian endian,
                     const std::uint32_t skip_offset, StreamInterface* stream,
                     std::uint32_t* makernote_offset,
                     TiffDirectory* makernote_ifd) {
  std::uint32_t makernote_length;
  if (!exif_ifd.GetOffsetAndLength(kExifTagMakernotes,
                                   tiff_directory::TIFF_TYPE_UNDEFINED,
                                   makernote_offset, &makernote_length)) {
    return false;
  }

  std::uint32_t next_ifd_offset;
  return ParseDirectory(*makernote_offset, *makernote_offset + skip_offset,
                        endian, {kTiffTagImageWidth, kOlymTagCameraSettings,
                                 kOlymTagRawProcessing, kPentaxTagColorSpace},
                        stream, makernote_ifd, &next_ifd_offset);
}

bool GetCameraSettingsIfd(const TiffDirectory& makernote_ifd,
                          const std::uint32_t makernote_offset,
                          const Endian endian, StreamInterface* stream,
                          TiffDirectory* camera_settings_ifd) {
  std::uint32_t camera_settings_offset;
  std::uint32_t camera_settings_length;
  if (!makernote_ifd.GetOffsetAndLength(
          kOlymTagCameraSettings, tiff_directory::TIFF_IFD,
          &camera_settings_offset, &camera_settings_length)) {
    return false;
  }

  std::uint32_t next_ifd_offset;
  if (!Get32u(stream, camera_settings_offset, endian,
              &camera_settings_offset)) {
    return false;
  }
  return ParseDirectory(makernote_offset,
                        makernote_offset + camera_settings_offset, endian,
                        {kTiffTagBitsPerSample, kTiffTagImageLength}, stream,
                        camera_settings_ifd, &next_ifd_offset);
}

bool GetRawProcessingIfd(const TagSet& desired_tags,
                         const TiffDirectory& makernote_ifd,
                         const std::uint32_t makernote_offset,
                         const Endian endian, StreamInterface* stream,
                         TiffDirectory* raw_processing_ifd) {
  std::uint32_t raw_processing_offset;
  std::uint32_t raw_processing_length;
  if (!makernote_ifd.GetOffsetAndLength(
          kOlymTagRawProcessing, tiff_directory::TIFF_IFD,
          &raw_processing_offset, &raw_processing_length)) {
    return false;
  }

  std::uint32_t next_ifd_offset;
  if (!Get32u(stream, raw_processing_offset, endian, &raw_processing_offset)) {
    return false;
  }

  return ParseDirectory(
      makernote_offset, makernote_offset + raw_processing_offset, endian,
      desired_tags, stream, raw_processing_ifd, &next_ifd_offset);
}

// Retrieves the preview image offset and length from the camera settings and
// the 'full_width' and 'full_height' from the raw processing ifd in 'stream'.
// Returns false if anything is wrong.
bool GetOlympusPreviewImage(StreamInterface* stream,
                            PreviewImageData* preview_image_data) {
  Endian endian;
  if (!GetEndianness(0 /* tiff offset */, stream, &endian)) {
    return false;
  }

  TiffDirectory exif_ifd(endian);
  if (!GetExifIfd(endian, stream, &exif_ifd)) {
    return false;
  }

  std::uint32_t makernote_offset;
  TiffDirectory makernote_ifd(endian);
  const std::uint32_t kSkipMakernoteStart = 12;
  if (!GetMakernoteIfd(exif_ifd, endian, kSkipMakernoteStart, stream,
                       &makernote_offset, &makernote_ifd)) {
    return false;
  }

  const std::uint32_t kThumbnailTag = 0x0100;
  if (makernote_ifd.Has(kThumbnailTag)) {
    if (!makernote_ifd.GetOffsetAndLength(
            kThumbnailTag, tiff_directory::TIFF_TYPE_UNDEFINED,
            &preview_image_data->thumbnail.offset,
            &preview_image_data->thumbnail.length)) {
      return false;
    }
  }

  TiffDirectory camera_settings_ifd(endian);
  if (!GetCameraSettingsIfd(makernote_ifd, makernote_offset, endian, stream,
                            &camera_settings_ifd)) {
    return false;
  }

  const std::uint32_t kPreviewOffset = 0x0101;
  const std::uint32_t kPreviewLength = 0x0102;
  if (!camera_settings_ifd.Has(kPreviewOffset) ||
      !camera_settings_ifd.Has(kPreviewLength)) {
    return false;
  }

  camera_settings_ifd.Get(kPreviewOffset, &preview_image_data->preview.offset);
  preview_image_data->preview.offset += makernote_offset;
  camera_settings_ifd.Get(kPreviewLength, &preview_image_data->preview.length);

  // Get the crop size from the raw processing ifd.
  TiffDirectory raw_processing_ifd(endian);
  if (!GetRawProcessingIfd({kOlymTagAspectFrame}, makernote_ifd,
                           makernote_offset, endian, stream,
                           &raw_processing_ifd)) {
    return false;
  }

  if (raw_processing_ifd.Has(kOlymTagAspectFrame)) {
    std::vector<std::uint32_t> aspect_frame(4);
    if (raw_processing_ifd.Get(kOlymTagAspectFrame, &aspect_frame) &&
        aspect_frame[2] > aspect_frame[0] &&
        aspect_frame[3] > aspect_frame[1]) {
      preview_image_data->full_width = aspect_frame[2] - aspect_frame[0] + 1;
      preview_image_data->full_height = aspect_frame[3] - aspect_frame[1] + 1;
      if (preview_image_data->full_width < preview_image_data->full_height) {
        std::swap(preview_image_data->full_width,
                  preview_image_data->full_height);
      }
    }
  }

  return true;
}

bool PefGetColorSpace(StreamInterface* stream,
                      PreviewImageData* preview_image_data) {
  Endian endian;
  if (!GetEndianness(0 /* tiff offset */, stream, &endian)) {
    return false;
  }

  TiffDirectory exif_ifd(endian);
  if (!GetExifIfd(endian, stream, &exif_ifd)) {
    return false;
  }

  std::uint32_t makernote_offset;
  TiffDirectory makernote_ifd(endian);
  const std::uint32_t kSkipMakernoteStart = 6;
  if (!GetMakernoteIfd(exif_ifd, endian, kSkipMakernoteStart, stream,
                       &makernote_offset, &makernote_ifd)) {
    return false;
  }
  if (makernote_ifd.Has(kPentaxTagColorSpace)) {
    std::uint32_t color_space;
    if (!makernote_ifd.Get(kPentaxTagColorSpace, &color_space)) {
      return false;
    }
    preview_image_data->color_space = color_space == 0
                                          ? PreviewImageData::kSrgb
                                          : PreviewImageData::kAdobeRgb;
  }
  return true;
}

bool RafGetOrientation(StreamInterface* stream, std::uint32_t* orientation) {
  // Parse the Fuji RAW header to get the offset and length of the preview
  // image, which contains the Exif information.
  const Endian endian = tiff_directory::kBigEndian;
  std::uint32_t preview_offset = 0;
  if (!Get32u(stream, kRafOffsetToPreviewOffset, endian, &preview_offset)) {
    return false;
  }

  const std::uint32_t exif_offset = preview_offset + 12;
  return GetExifOrientation(stream, exif_offset, orientation);
}

// Parses the Fuji Cfa header for the image width and height.
bool RafGetDimension(StreamInterface* stream, std::uint32_t* width,
                     std::uint32_t* height) {
  const Endian endian = tiff_directory::kBigEndian;
  std::uint32_t cfa_header_index = 0;  // actual position in the cfa header.
  std::uint32_t cfa_header_entries = 0;
  if (!Get32u(stream, 92 /* cfa header offset */, endian, &cfa_header_index) ||
      !Get32u(stream, cfa_header_index, endian, &cfa_header_entries)) {
    return false;
  }

  // Add 4 to point to the actual read position in the cfa header.
  cfa_header_index += 4;

  for (std::uint32_t i = 0; i < cfa_header_entries; ++i) {
    std::uint16_t id = 0;
    std::uint16_t length = 0;
    if (!Get16u(stream, cfa_header_index, endian, &id) ||
        !Get16u(stream, cfa_header_index + 2, endian, &length)) {
      return false;
    }

    std::uint16_t tmp_width = 0;
    std::uint16_t tmp_height = 0;
    if (id == 0x0111 /* tags the crop dimensions */ &&
        Get16u(stream, cfa_header_index + 4, endian, &tmp_height) &&
        Get16u(stream, cfa_header_index + 6, endian, &tmp_width)) {
      *width = tmp_width;
      *height = tmp_height;
      return true;
    }
    cfa_header_index += 4u + length;
  }
  return false;
}

Error ArwGetPreviewData(StreamInterface* stream,
                        PreviewImageData* preview_image_data) {
  const TagSet extended_tags = {kExifTagHeight, kExifTagWidth,
                                kTiffTagJpegByteCount, kTiffTagJpegOffset,
                                kTiffTagSubIfd};

  GetThumbnailOffsetAndLength(TagSet(), stream, preview_image_data);

  const std::uint32_t kNumberOfIfds = 1;
  if (GetPreviewData(extended_tags, kNumberOfIfds, stream,
                     preview_image_data)) {
    return kOk;
  }
  return kFail;
}

Error Cr2GetPreviewData(StreamInterface* stream,
                        PreviewImageData* preview_image_data) {
  const TagSet extended_tags = {kExifTagHeight, kExifTagWidth,
                                kTiffTagStripByteCounts, kTiffTagStripOffsets};

  GetThumbnailOffsetAndLength(TagSet(), stream, preview_image_data);

  const std::uint32_t kNumberOfIfds = 1;
  if (GetPreviewData(extended_tags, kNumberOfIfds, stream,
                     preview_image_data)) {
    return kOk;
  }
  return kFail;
}

Error DngGetPreviewData(StreamInterface* stream,
                        PreviewImageData* preview_image_data) {
  // Some thumbnails from DngCreator are larger than the specified 256 pixel.
  const int kDngThumbnailMaxDimension = 512;

  const TagSet extended_tags = {
      kExifTagDefaultCropSize, kTiffTagImageWidth,   kTiffTagImageLength,
      kTiffTagStripByteCounts, kTiffTagStripOffsets, kTiffTagSubIfd};

  TiffContent tiff_content;
  const std::uint32_t kNumberOfIfds = 3;
  if (!GetPreviewData(extended_tags, 0, kNumberOfIfds, stream, &tiff_content,
                      preview_image_data)) {
    return kFail;
  }

  const TiffDirectory& tiff_directory = tiff_content.tiff_directory[0];

  if (!GetFullCropDimension(tiff_directory, &preview_image_data->full_width,
                            &preview_image_data->full_height)) {
    return kFail;
  }

  // Find the jpeg compressed thumbnail and preview image.
  Image preview;
  Image thumbnail;

  // Search for images in IFD0
  Image temp_image;
  if (GetImageData(tiff_directory, stream, &temp_image)) {
    if (IsThumbnail(temp_image, kDngThumbnailMaxDimension)) {
      thumbnail = temp_image;
    } else if (temp_image.format == Image::kJpegCompressed) {
      preview = temp_image;
    }
  }

  // Search for images in other IFDs
  for (const auto& ifd : tiff_directory.GetSubDirectories()) {
    if (GetImageData(ifd, stream, &temp_image)) {
      // Try to find the largest thumbnail/preview.
      if (IsThumbnail(temp_image, kDngThumbnailMaxDimension)) {
        if (temp_image > thumbnail) {
          thumbnail = temp_image;
        }
      } else {
        if (temp_image > preview &&
            temp_image.format == Image::kJpegCompressed) {
          preview = temp_image;
        }
      }
    }
  }
  preview_image_data->preview = preview;
  preview_image_data->thumbnail = thumbnail;

  return kOk;
}

Error NefGetPreviewData(StreamInterface* stream,
                        PreviewImageData* preview_image_data) {
  const TagSet extended_tags = {kTiffTagImageWidth,      kTiffTagImageLength,
                                kTiffTagJpegByteCount,   kTiffTagJpegOffset,
                                kTiffTagStripByteCounts, kTiffTagStripOffsets,
                                kTiffTagSubIfd};
  const std::uint32_t kNumberOfIfds = 2;
  if (!GetPreviewData(extended_tags, kNumberOfIfds, stream,
                      preview_image_data)) {
    return kFail;
  }

  if (preview_image_data->thumbnail.length == 0) {
    PreviewImageData thumbnail_data;
    GetThumbnailOffsetAndLength(TagSet(), stream, &thumbnail_data);
    preview_image_data->thumbnail = thumbnail_data.thumbnail;
  }

  // The Nikon RAW data provides the dimensions of the sensor image, which are
  // slightly larger than the dimensions of the preview image. In order to
  // determine the correct full width and height of the image, the preview image
  // size needs to be taken into account. Based on experiments the preview image
  // dimensions must be at least 90% of the sensor image dimensions to let it be
  // a full size preview image.
  if (preview_image_data->preview.length > 0) {  // when preview image exists
    const float kEpsilon = 0.9f;

    std::uint16_t width;
    std::uint16_t height;
    if (!GetJpegDimensions(preview_image_data->preview.offset, stream, &width,
                           &height) ||
        preview_image_data->full_width == 0 ||
        preview_image_data->full_height == 0) {
      return kUnsupported;
    }

    if (static_cast<float>(width) /
                static_cast<float>(preview_image_data->full_width) >
            kEpsilon ||
        static_cast<float>(height) /
                static_cast<float>(preview_image_data->full_height) >
            kEpsilon) {
      preview_image_data->full_width = width;
      preview_image_data->full_height = height;
    }
  }
  return kOk;
}

Error OrfGetPreviewData(StreamInterface* stream,
                        PreviewImageData* preview_image_data) {
  if (!GetExifData(0, stream, preview_image_data)) {
    return kFail;
  }
  // Omit errors, because some images do not contain any preview data.
  GetOlympusPreviewImage(stream, preview_image_data);
  return kOk;
}

Error PefGetPreviewData(StreamInterface* stream,
                        PreviewImageData* preview_image_data) {
  const TagSet extended_tags = {kTiffTagImageWidth, kTiffTagImageLength,
                                kTiffTagJpegByteCount, kTiffTagJpegOffset,
                                kTiffTagSubIfd};
  const std::uint32_t kNumberOfIfds = 3;
  if (!GetPreviewData(extended_tags, kNumberOfIfds, stream,
                      preview_image_data) ||
      !PefGetColorSpace(stream, preview_image_data)) {
    return kFail;
  }

  PreviewImageData thumbnail_data;
  GetThumbnailOffsetAndLength(TagSet(), stream, &thumbnail_data);
  preview_image_data->thumbnail = thumbnail_data.thumbnail;

  return kOk;
}

Error RafGetPreviewData(StreamInterface* stream,
                        PreviewImageData* preview_image_data) {
  // Parse the Fuji RAW header to get the offset and length of the preview
  // image, which contains the Exif information.
  const Endian endian = tiff_directory::kBigEndian;
  std::uint32_t preview_offset = 0;
  std::uint32_t preview_length = 0;
  if (!Get32u(stream, kRafOffsetToPreviewOffset, endian, &preview_offset) ||
      !Get32u(stream, kRafOffsetToPreviewOffset + 4, endian, &preview_length)) {
    return kFail;
  }

  if (!RafGetDimension(stream, &preview_image_data->full_width,
                       &preview_image_data->full_height)) {
    return kFail;
  }

  if (preview_length > 0) {  // when preview image exists
    // Parse the Exif information from the preview image.
    const std::uint32_t exif_offset = preview_offset + 12;
    if (!GetExifData(exif_offset, stream, preview_image_data)) {
      return kFail;
    }
  }

  // Merge the Exif data with the RAW data to form the preview_image_data.
  preview_image_data->thumbnail.offset += 160;  // Skip the cfa header.
  preview_image_data->preview.offset = preview_offset;
  preview_image_data->preview.length = preview_length;
  return kOk;
}

Error Rw2GetPreviewData(StreamInterface* stream,
                        PreviewImageData* preview_image_data) {
  const TagSet extended_tags = {kPanaTagTopBorder,     kPanaTagLeftBorder,
                                kPanaTagBottomBorder,  kPanaTagRightBorder,
                                kPanaTagIso,           kPanaTagJpegImage,
                                kTiffTagJpegByteCount, kTiffTagJpegOffset};
  // Parse the RAW data to get the ISO, offset and length of the preview image,
  // which contains the Exif information.
  const std::uint32_t kNumberOfIfds = 1;
  PreviewImageData preview_data;
  if (!GetPreviewData(extended_tags, kNumberOfIfds, stream, &preview_data)) {
    return kFail;
  }

  if (preview_data.preview.length > 0) {  // when preview image exists
    // Parse the Exif information from the preview image.
    const std::uint32_t exif_offset = preview_data.preview.offset + 12;
    if (!GetExifData(exif_offset, stream, preview_image_data)) {
      return kFail;
    }
    preview_image_data->thumbnail.offset += exif_offset;
  }

  // Merge the Exif data with the RAW data to form the preview_image_data.
  preview_image_data->preview = preview_data.preview;
  preview_image_data->iso = preview_data.iso;
  preview_image_data->full_width = preview_data.full_width;
  preview_image_data->full_height = preview_data.full_height;

  return kOk;
}

Error SrwGetPreviewData(StreamInterface* stream,
                        PreviewImageData* preview_image_data) {
  GetThumbnailOffsetAndLength({kTiffTagSubIfd}, stream, preview_image_data);

  const TagSet extended_tags = {kExifTagWidth, kExifTagHeight,
                                kTiffTagJpegByteCount, kTiffTagJpegOffset,
                                kTiffTagSubIfd};
  const std::uint32_t kNumberOfIfds = 1;
  if (!GetPreviewData(extended_tags, kNumberOfIfds, stream,
                      preview_image_data)) {
    return kFail;
  }
  return kOk;
}

}  // namespace

size_t BytesRequiredForIsRaw() {
  return image_type_recognition::GetNumberOfBytesForIsRawLite();
}

bool IsRaw(StreamInterface* data) {
  const size_t bytes = BytesRequiredForIsRaw();
  if (data == nullptr) {
    return false;
  }

  // Read required number of bytes into a vector.
  std::vector<std::uint8_t> file_header(bytes);
  if (data->GetData(0, file_header.size(), file_header.data()) != kOk) {
    return false;
  }

  RangeCheckedBytePtr data_buffer(file_header.data(), file_header.size());

  return image_type_recognition::IsRawLite(data_buffer);
}

Error GetPreviewImageData(StreamInterface* data,
                          PreviewImageData* preview_image_data) {
  const size_t bytes = BytesRequiredForIsRaw();
  if (data == nullptr || bytes == 0) {
    return kFail;
  }

  std::vector<std::uint8_t> file_header(bytes);
  Error error = data->GetData(0, file_header.size(), file_header.data());
  if (error != kOk) {
    return error;
  }
  RangeCheckedBytePtr header_buffer(file_header.data(), file_header.size());

  switch (RecognizeRawImageTypeLite(header_buffer)) {
    case image_type_recognition::kArwImage:
      return ArwGetPreviewData(data, preview_image_data);
    case image_type_recognition::kCr2Image:
      return Cr2GetPreviewData(data, preview_image_data);
    case image_type_recognition::kDngImage:
      return DngGetPreviewData(data, preview_image_data);
    case image_type_recognition::kNefImage:
    case image_type_recognition::kNrwImage:
      return NefGetPreviewData(data, preview_image_data);
    case image_type_recognition::kOrfImage:
      return OrfGetPreviewData(data, preview_image_data);
    case image_type_recognition::kPefImage:
      return PefGetPreviewData(data, preview_image_data);
    case image_type_recognition::kRafImage:
      return RafGetPreviewData(data, preview_image_data);
    case image_type_recognition::kRw2Image:
      return Rw2GetPreviewData(data, preview_image_data);
    case image_type_recognition::kSrwImage:
      return SrwGetPreviewData(data, preview_image_data);
    default:
      return kUnsupported;
  }
}

bool GetDngInformation(StreamInterface* data, std::uint32_t* width,
                       std::uint32_t* height,
                       std::vector<std::uint32_t>* cfa_pattern_dim) {
  // If IFD0 contains already the full dimensions we do not parse into the sub
  // IFD.
  if (!GetDngInformation({}, data, width, height, cfa_pattern_dim)) {
    return GetDngInformation({kTiffTagSubIfd}, data, width, height,
                             cfa_pattern_dim);
  }
  return true;
}

bool GetOrientation(StreamInterface* data, std::uint32_t* orientation) {
  using image_type_recognition::GetNumberOfBytesForIsOfType;
  using image_type_recognition::IsOfType;

  std::vector<std::uint8_t> file_header(
      GetNumberOfBytesForIsOfType(image_type_recognition::kRafImage));
  if (data->GetData(0, file_header.size(), file_header.data()) != kOk) {
    return false;
  }

  // For RAF files a special routine is necessary to get orientation. For others
  // the general approach is sufficient.
  if (IsOfType(RangeCheckedBytePtr(file_header.data(), file_header.size()),
               image_type_recognition::kRafImage)) {
    return RafGetOrientation(data, orientation);
  } else {
    return GetExifOrientation(data, 0 /* offset */, orientation);
  }
}

std::vector<std::string> SupportedExtensions() {
  return {"ARW", "CR2", "DNG", "NEF", "NRW", "ORF", "PEF", "RAF", "RW2", "SRW"};
}

}  // namespace piex
