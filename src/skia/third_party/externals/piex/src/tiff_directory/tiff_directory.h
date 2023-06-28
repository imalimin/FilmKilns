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
// TiffDirectory contains an abstraction of an image file directory (IFD) as
// proposed by the TIFF specification.

#ifndef PIEX_TIFF_DIRECTORY_TIFF_DIRECTORY_H_
#define PIEX_TIFF_DIRECTORY_TIFF_DIRECTORY_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace piex {
namespace tiff_directory {

enum Endian {
  kLittleEndian = 0,
  kBigEndian = 1,
};

struct Rational {
  std::uint32_t numerator;
  std::uint32_t denominator;
};

struct SRational {
  std::int32_t numerator;
  std::int32_t denominator;
};

enum TiffTypes {
  TIFF_TYPE_NONE = 0,
  TIFF_TYPE_BYTE,      /* 8bit unsigned */
  TIFF_TYPE_ASCII,     /* Ascii string (terminated by \0) */
  TIFF_TYPE_SHORT,     /* 16bit unsigned  */
  TIFF_TYPE_LONG,      /* 32bit unsigned  */
  TIFF_TYPE_RATIONAL,  /* 32bit/32bit unsigned  */
  TIFF_TYPE_SBYTE,     /* 8bit signed */
  TIFF_TYPE_UNDEFINED, /* undefined (depend of tag) */
  TIFF_TYPE_SSHORT,    /* 16bit signed*/
  TIFF_TYPE_SLONG,     /* 32bit signed  */
  TIFF_TYPE_SRATIONAL, /* 32bit/32bit signed */
  TIFF_TYPE_FLOAT,     /* 32-bit IEEE float */
  TIFF_TYPE_DOUBLE,    /* 64-bit IEEE float */
  TIFF_IFD,            /* IFD type */
};

// The TiffDirectory class stores all information necessary to interpret TIFF
// tags and manages also potential sub directories.
class TiffDirectory {
 public:
  typedef std::uint32_t Tag;
  typedef std::uint32_t Type;

  explicit TiffDirectory(Endian endianness);

  // Returns true if the directory contains the specified tag.
  bool Has(const Tag tag) const;

  // Gets the value of a tag of byte vector type.
  // Returns false if the tag is not part of the directory or if the
  // type is not BYTE or UNDEFINED.
  bool Get(const Tag tag, std::vector<std::uint8_t>* value) const;

  // Gets the value of a tag of type "ASCII".
  // Returns false if the tag is not part of the directory or if its
  // type is not ASCII.
  // If *err is not equal to ERR_OK initially, this method does nothing.
  bool Get(const Tag tag, std::string* value) const;

  // Gets the value of a tag of type "SHORT" or "LONG".
  // Returns false
  // - if the tag is not part of the directory or
  // - if the type is not SHORT or LONG, or
  // - if, for the non-vector version, the number of elements is unequal to 1.
  bool Get(const Tag tag, std::uint32_t* value) const;
  bool Get(const Tag tag, std::vector<std::uint32_t>* value) const;

  // Gets the value of a tag of type "SHORT", "LONG" or "RATIONAL".
  // Returns false
  // - if the tag is not part of the directory or
  // - if the type is not SHORT, LONG or RATIONAL, or
  // - if, for the non-vector version, the number of elements is unequal to 1.
  bool Get(const Tag tag, Rational* value) const;
  bool Get(const Tag tag, std::vector<Rational>* value) const;

  // Gets the value of a tag of type "SSHORT", "SLONG" or "SRATIONAL".
  // Returns false
  // - if the tag is not part of the directory or
  // - if the type is not SSHORT, SLONG or SRATIONAL, or
  // - if, for the non-vector version, the number of elements is unequal to 1.
  bool Get(const Tag tag, SRational* value) const;
  bool Get(const Tag tag, std::vector<SRational>* value) const;

  // Gets the 'offset' to the value data in the file and its 'length' in bytes.
  // Returns false if the 'tag' is not part of the directory or if its type does
  // not match the desired 'type'.
  bool GetOffsetAndLength(const Tag tag, const Type type, std::uint32_t* offset,
                          std::uint32_t* length) const;

  // Adds a tag to the directory, setting its type, number of elements
  // ('count'), the offset to the binary data in the file ('offset') and the
  // associated binary data ('value'). The binary data is encoded according to
  // the TIFF specification with the endianness that was specified when this
  // object was constructed. The caller must ensure that the size of 'value' and
  // the data it contains are consistent with 'type' and 'count'. It is not
  // legal to call this method with a tag that is already contained in the
  // directory.
  void AddEntry(const Tag tag, const Type type, const std::uint32_t count,
                const std::uint32_t offset,
                const std::vector<std::uint8_t>& value);

  // Add a subdirectory to the directory.
  void AddSubDirectory(const TiffDirectory& sub_directory);

  // Returns a vector of all subdirectories contained in this directory.
  const std::vector<TiffDirectory>& GetSubDirectories() const;

 private:
  struct DirectoryEntry {
    Type type;
    std::uint32_t count;   // The number of values of type, not a byte count.
    std::uint32_t offset;  // Offset of the entry's data in the file. '0' means
                           // the offset is not set.
    std::vector<std::uint8_t> value;
  };

  const DirectoryEntry* Find(const Tag tag) const;

  std::map<Tag, DirectoryEntry> directory_entries_;
  std::vector<Tag> tag_order_;
  std::vector<TiffDirectory> sub_directories_;
  Endian endian_;
};

// Returns the number of bytes a single value of 'type' requires; this is
// guaranteed to be in the range of 0 to 8.
// Returns 0 if 'type' is TIFF_TYPE_NONE or invalid. Sets 'success' to false if
// 'type' is invalid. If you are not interested in 'success' you can set it to
// a nullptr.
size_t SizeOfType(const TiffDirectory::Type type, bool* success);

}  // namespace tiff_directory
}  // namespace piex

#endif  // PIEX_TIFF_DIRECTORY_TIFF_DIRECTORY_H_
