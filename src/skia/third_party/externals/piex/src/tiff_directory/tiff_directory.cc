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

#include "src/tiff_directory/tiff_directory.h"

#include <assert.h>
#include <climits>

#include "src/binary_parse/range_checked_byte_ptr.h"

namespace piex {
namespace tiff_directory {
namespace {

using binary_parse::Get16s;
using binary_parse::Get16u;
using binary_parse::Get32s;
using binary_parse::Get32u;
using binary_parse::MemoryStatus;
using binary_parse::RANGE_CHECKED_BYTE_SUCCESS;
using binary_parse::RangeCheckedBytePtr;

}  // namespace

TiffDirectory::TiffDirectory(Endian endian) : endian_(endian) {}

bool TiffDirectory::Has(const Tag tag) const {
  return directory_entries_.count(tag) == 1;
}

bool TiffDirectory::Get(const Tag tag, std::vector<std::uint8_t>* value) const {
  const DirectoryEntry* directory_entry = Find(tag);
  if (directory_entry == NULL ||
      (directory_entry->type != TIFF_TYPE_BYTE &&
       directory_entry->type != TIFF_TYPE_UNDEFINED)) {
    return false;
  }

  *value = directory_entry->value;
  return true;
}

bool TiffDirectory::Get(const Tag tag, std::string* value) const {
  const DirectoryEntry* directory_entry = Find(tag);
  if (directory_entry == NULL || directory_entry->type != TIFF_TYPE_ASCII) {
    return false;
  }
  *value =
      std::string(directory_entry->value.begin(), directory_entry->value.end());
  return true;
}

bool TiffDirectory::Get(const Tag tag, std::uint32_t* value) const {
  std::vector<std::uint32_t> my_values;
  if (!Get(tag, &my_values) || my_values.size() != 1) {
    return false;
  }
  *value = my_values[0];
  return true;
}

bool TiffDirectory::Get(const Tag tag,
                        std::vector<std::uint32_t>* value) const {
  const DirectoryEntry* directory_entry = Find(tag);
  if (directory_entry == NULL || (directory_entry->type != TIFF_TYPE_SHORT &&
                                  directory_entry->type != TIFF_TYPE_LONG)) {
    return false;
  }

  RangeCheckedBytePtr value_ptr(&directory_entry->value[0],
                                directory_entry->value.size());
  std::vector<std::uint32_t> my_value(directory_entry->count);
  const bool is_big_endian = (endian_ == kBigEndian);

  MemoryStatus err = RANGE_CHECKED_BYTE_SUCCESS;
  for (std::uint32_t c = 0; c < directory_entry->count; ++c) {
    if (directory_entry->type == TIFF_TYPE_SHORT) {
      my_value[c] = Get16u(value_ptr + c * 2, is_big_endian, &err);
    } else {
      my_value[c] = Get32u(value_ptr + c * 4, is_big_endian, &err);
    }
  }
  if (err != RANGE_CHECKED_BYTE_SUCCESS) {
    return false;
  }

  *value = my_value;
  return true;
}

bool TiffDirectory::Get(const Tag tag, Rational* value) const {
  std::vector<Rational> my_values;
  if (!Get(tag, &my_values) || my_values.size() != 1) {
    return false;
  }
  *value = my_values[0];
  return true;
}

bool TiffDirectory::Get(const Tag tag, std::vector<Rational>* value) const {
  const DirectoryEntry* directory_entry = Find(tag);
  if (directory_entry == NULL ||
      (directory_entry->type != TIFF_TYPE_SHORT &&
       directory_entry->type != TIFF_TYPE_LONG &&
       directory_entry->type != TIFF_TYPE_RATIONAL)) {
    return false;
  }

  RangeCheckedBytePtr value_ptr(&directory_entry->value[0],
                                directory_entry->value.size());
  std::vector<Rational> my_value(directory_entry->count);
  const bool is_big_endian = (endian_ == kBigEndian);

  MemoryStatus err = RANGE_CHECKED_BYTE_SUCCESS;
  for (std::uint32_t c = 0; c < directory_entry->count; ++c) {
    switch (directory_entry->type) {
      case TIFF_TYPE_SHORT: {
        my_value[c].numerator = Get16u(value_ptr + c * 2, is_big_endian, &err);
        my_value[c].denominator = 1;
        break;
      }
      case TIFF_TYPE_LONG: {
        my_value[c].numerator = Get32u(value_ptr + c * 4, is_big_endian, &err);
        my_value[c].denominator = 1;
        break;
      }
      case TIFF_TYPE_RATIONAL: {
        my_value[c].numerator = Get32u(value_ptr + c * 8, is_big_endian, &err);
        my_value[c].denominator =
            Get32u(value_ptr + c * 8 + 4, is_big_endian, &err);
        if (my_value[c].denominator == 0) {
          return false;
        }
        break;
      }
    }
  }
  if (err != RANGE_CHECKED_BYTE_SUCCESS) {
    return false;
  }

  *value = my_value;
  return true;
}

bool TiffDirectory::Get(const Tag tag, SRational* value) const {
  std::vector<SRational> my_values;
  if (!Get(tag, &my_values) || my_values.size() != 1) {
    return false;
  }
  *value = my_values[0];
  return true;
}

bool TiffDirectory::Get(const Tag tag, std::vector<SRational>* value) const {
  const DirectoryEntry* directory_entry = Find(tag);
  if (directory_entry == NULL ||
      (directory_entry->type != TIFF_TYPE_SSHORT &&
       directory_entry->type != TIFF_TYPE_SLONG &&
       directory_entry->type != TIFF_TYPE_SRATIONAL)) {
    return false;
  }

  RangeCheckedBytePtr value_ptr(&directory_entry->value[0],
                                directory_entry->value.size());
  std::vector<SRational> my_value(directory_entry->count);
  const bool is_big_endian = (endian_ == kBigEndian);

  MemoryStatus err = RANGE_CHECKED_BYTE_SUCCESS;
  for (std::uint32_t c = 0; c < directory_entry->count; ++c) {
    switch (directory_entry->type) {
      case TIFF_TYPE_SSHORT: {
        my_value[c].numerator = Get16s(value_ptr + c * 2, is_big_endian, &err);
        my_value[c].denominator = 1;
        break;
      }
      case TIFF_TYPE_SLONG: {
        my_value[c].numerator = Get32s(value_ptr + c * 4, is_big_endian, &err);
        my_value[c].denominator = 1;
        break;
      }
      case TIFF_TYPE_SRATIONAL: {
        my_value[c].numerator = Get32s(value_ptr + c * 8, is_big_endian, &err);
        my_value[c].denominator =
            Get32s(value_ptr + c * 8 + 4, is_big_endian, &err);
        if (my_value[c].denominator == 0) {
          return false;
        }
        break;
      }
    }
  }
  if (err != RANGE_CHECKED_BYTE_SUCCESS) {
    return false;
  }

  *value = my_value;
  return true;
}

bool TiffDirectory::GetOffsetAndLength(const Tag tag, const Type type,
                                       std::uint32_t* offset,
                                       std::uint32_t* length) const {
  const DirectoryEntry* directory_entry = Find(tag);
  if (directory_entry == NULL || directory_entry->type != type) {
    return false;
  }
  *offset = directory_entry->offset;
  *length = static_cast<std::uint32_t>(directory_entry->value.size());
  return true;
}

void TiffDirectory::AddEntry(const Tag tag, const Type type,
                             const std::uint32_t count,
                             const std::uint32_t offset,
                             const std::vector<std::uint8_t>& value) {
  assert(SizeOfType(type, NULL /* success */) * count == value.size());

  const DirectoryEntry directory_entry = {type, count, offset, value};
  directory_entries_[tag] = directory_entry;
  tag_order_.push_back(tag);
}

void TiffDirectory::AddSubDirectory(const TiffDirectory& sub_directory) {
  sub_directories_.push_back(sub_directory);
}

const std::vector<TiffDirectory>& TiffDirectory::GetSubDirectories() const {
  return sub_directories_;
}

const TiffDirectory::DirectoryEntry* TiffDirectory::Find(const Tag tag) const {
  std::map<Tag, DirectoryEntry>::const_iterator iter =
      directory_entries_.find(tag);
  if (iter == directory_entries_.end()) {
    return NULL;
  }
  return &iter->second;
}

size_t SizeOfType(const TiffDirectory::Type type, bool* success) {
  switch (type) {
    case TIFF_TYPE_BYTE:
    case TIFF_TYPE_ASCII:
    case TIFF_TYPE_SBYTE:
    case TIFF_TYPE_UNDEFINED:
      return 1;
    case TIFF_TYPE_SHORT:
    case TIFF_TYPE_SSHORT:
      return 2;
    case TIFF_TYPE_LONG:
    case TIFF_TYPE_SLONG:
    case TIFF_TYPE_FLOAT:
    case TIFF_IFD:
      return 4;
    case TIFF_TYPE_RATIONAL:
    case TIFF_TYPE_SRATIONAL:
    case TIFF_TYPE_DOUBLE:
      return 8;
  }

  if (success != NULL) {
    *success = false;
  }
  return 0;
}

}  // namespace tiff_directory
}  // namespace piex
