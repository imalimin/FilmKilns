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

#ifndef PIEX_TIFF_PARSER_H_
#define PIEX_TIFF_PARSER_H_

#include <cstdint>
#include <memory>
#include <set>
#include <vector>

#include "src/piex_types.h"
#include "src/tiff_directory/tiff_directory.h"

namespace piex {

// Specifies the maximum number of pixels for thumbnails in each direction.
const int kThumbnailMaxDimension = 512;

// Specifies all tags that might be of interest to get the preview data.
enum GpsTags {
  kGpsTagLatitudeRef = 1,
  kGpsTagLatitude = 2,
  kGpsTagLongitudeRef = 3,
  kGpsTagLongitude = 4,
  kGpsTagAltitudeRef = 5,
  kGpsTagAltitude = 6,
  kGpsTagTimeStamp = 7,
  kGpsTagDateStamp = 29,
};

enum TiffTags {
  kExifTagColorSpace = 0xA001,
  kExifTagDateTimeOriginal = 0x9003,
  kExifTagDefaultCropSize = 0xC620,
  kExifTagExposureTime = 0x829a,
  kExifTagFnumber = 0x829d,
  kExifTagFocalLength = 0x920A,
  kExifTagGps = 0x8825,
  kExifTagHeight = 0xA003,
  kExifTagIsoSpeed = 0x8827,
  kExifTagMakernotes = 0x927C,
  kExifTagWidth = 0xA002,
  kOlymTagAspectFrame = 0x1113,
  kOlymTagCameraSettings = 0x2020,
  kOlymTagRawProcessing = 0x2040,
  kPanaTagBottomBorder = 0x006,
  kPanaTagIso = 0x0017,
  kPanaTagJpegImage = 0x002E,
  kPanaTagLeftBorder = 0x0005,
  kPanaTagRightBorder = 0x007,
  kPanaTagTopBorder = 0x0004,
  kPentaxTagColorSpace = 0x0037,
  kTiffTagArtist = 0x013B,
  kTiffTagBitsPerSample = 0x0102,
  kTiffTagCfaPatternDim = 0x828D,
  kTiffTagCompression = 0x0103,
  kTiffTagDateTime = 0x0132,
  kTiffTagExifIfd = 0x8769,
  kTiffTagImageDescription = 0x010E,
  kTiffTagImageLength = 0x0101,
  kTiffTagImageWidth = 0x0100,
  kTiffTagJpegByteCount = 0x0202,
  kTiffTagJpegOffset = 0x0201,
  kTiffTagMake = 0x010F,
  kTiffTagModel = 0x0110,
  kTiffTagOrientation = 0x0112,
  kTiffTagPhotometric = 0x0106,
  kTiffTagPlanarConfig = 0x011C,
  kTiffTagResolutionUnit = 0x0128,
  kTiffTagRowsPerStrip = 0x0116,
  kTiffTagSamplesPerPixel = 0x0115,
  kTiffTagSoftware = 0x0131,
  kTiffTagStripByteCounts = 0x0117,
  kTiffTagStripOffsets = 0x0111,
  kTiffTagSubFileType = 0x00FE,
  kTiffTagSubIfd = 0x014A,
  kTiffTagTileByteCounts = 0x0145,
  kTiffTagTileLength = 0x0143,
  kTiffTagTileOffsets = 0x0144,
  kTiffTagTileWidth = 0x0142,
  kTiffTagXresolution = 0x011A,
  kTiffTagYresolution = 0x011B,
};

typedef std::set<tiff_directory::TiffDirectory::Tag> TagSet;
typedef std::vector<tiff_directory::TiffDirectory> IfdVector;

struct TiffContent {
  IfdVector tiff_directory;
  std::unique_ptr<tiff_directory::TiffDirectory> exif_directory;
  std::unique_ptr<tiff_directory::TiffDirectory> gps_directory;
};

// Reads 2 bytes, an unsigned 16bit from 'stream' at a certain 'offset'. The
// bytes get swapped according to the desired endianness returning true on
// success. Returns false when something is wrong.
bool Get16u(StreamInterface* stream, const std::uint32_t offset,
            const tiff_directory::Endian& endian, std::uint16_t* value);

// Reads 4 bytes, an unsigned 32bit 'value' from 'stream' at a certain 'offset'.
// The bytes get swapped according to the desired endianness returning true on
// success. Returns false when something is wrong.
bool Get32u(StreamInterface* stream, const std::uint32_t offset,
            const tiff_directory::Endian& endian, std::uint32_t* value);

// Retrieves a byte vector of size 'length' from 'stream' beginning at some
// 'offset' reading the data in chunks of one MiB.
// If 'error' is not set to kOk the returned value is invalid.
std::vector<std::uint8_t> GetData(const size_t offset, const size_t length,
                                  StreamInterface* stream, Error* error);

// Retrieves the endianness of TIFF compliant data at 'tiff_offset' from
// 'stream' returning true on success. Returns false when something is wrong.
bool GetEndianness(const std::uint32_t tiff_offset, StreamInterface* stream,
                   tiff_directory::Endian* endian);

// Retrieves an image from tiff_directory. Return false when something is wrong.
bool GetImageData(const tiff_directory::TiffDirectory& tiff_directory,
                  StreamInterface* stream, Image* image);

// Retrieves the width and height from the jpeg image returning true on
// success. Returns false when something is wrong.
bool GetJpegDimensions(const std::uint32_t jpeg_offset, StreamInterface* stream,
                       std::uint16_t* width, std::uint16_t* height);

// According to Tiff/EP a thumbnail has max 256 pixels per dimension.
// http://standardsproposals.bsigroup.com/Home/getPDF/567
bool IsThumbnail(const Image& image,
                 const int max_dimension = kThumbnailMaxDimension);

// Parses through a Tiff IFD and writes all 'desired_tags' to a
// 'tiff_directory'.
// Returns false if something with the Tiff data is wrong.
bool ParseDirectory(const std::uint32_t tiff_offset,
                    const std::uint32_t ifd_offset,
                    const tiff_directory::Endian endian,
                    const TagSet& desired_tags, StreamInterface* stream,
                    tiff_directory::TiffDirectory* tiff_directory,
                    std::uint32_t* next_ifd_offset);

// Returns true if Exif orientation for the image can be obtained. False
// otherwise.
bool GetExifOrientation(StreamInterface* stream, const std::uint32_t offset,
                        std::uint32_t* orientation);

// Reads the width and height of the full resolution image. The tag groups are
// exclusive.
bool GetFullDimension32(const tiff_directory::TiffDirectory& tiff_directory,
                        std::uint32_t* width, std::uint32_t* height);

// Reads the width and height of the crop information if available.
// Returns false if an error occurred.
bool GetFullCropDimension(const tiff_directory::TiffDirectory& tiff_directory,
                          std::uint32_t* width, std::uint32_t* height);

// Enables us to parse through data that complies to the Tiff/EP specification.
class TiffParser {
 public:
  // The caller owns 'stream' and is responsible to keep it alive while the
  // TiffParser object is used.
  explicit TiffParser(StreamInterface* stream);
  TiffParser(StreamInterface* stream, const std::uint32_t offset);

  // Runs over the Tiff IFD, Exif IFD and subIFDs to get the preview image data.
  // Returns false if something with the Tiff tags is wrong.
  bool GetPreviewImageData(const TiffContent& tiff_content,
                           PreviewImageData* image_metadata);

  // Returns false if called more that once or something with the Tiff data is
  // wrong.
  bool Parse(const TagSet& desired_tags, const std::uint16_t max_number_ifds,
             TiffContent* tiff_content);

 private:
  // Disallow copy and assignment.
  TiffParser(const TiffParser&) = delete;
  TiffParser& operator=(const TiffParser&) = delete;

  bool ParseIfd(const std::uint32_t ifd_offset, const TagSet& desired_tags,
                const std::uint16_t max_number_ifds, IfdVector* tiff_directory);
  bool ParseGpsData(const tiff_directory::TiffDirectory* tiff_ifd,
                    TiffContent* tiff_content);

  StreamInterface* stream_ = nullptr;
  std::uint32_t tiff_offset_ = 0;
  tiff_directory::Endian endian_;
};

}  // namespace piex

#endif  // PIEX_TIFF_PARSER_H_
