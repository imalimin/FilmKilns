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
// LRU cache decorator for binary_parse::PagedByteArray subclasses.

#ifndef PIEX_BINARY_PARSE_CACHED_PAGED_BYTE_ARRAY_H_
#define PIEX_BINARY_PARSE_CACHED_PAGED_BYTE_ARRAY_H_

#include <mutex>
#include <vector>

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include "src/binary_parse/range_checked_byte_ptr.h"

namespace piex {
namespace binary_parse {

class CachedPagedByteArray : public PagedByteArray {
 public:
  // Decorates 'paged_byte_array' with a LRU cache layer of the size
  // 'cache_size'.
  explicit CachedPagedByteArray(const PagedByteArray* paged_byte_array,
                                size_t cache_size);

  virtual size_t length() const { return paged_byte_array_->length(); }

  virtual size_t pageSize() const { return paged_byte_array_->pageSize(); }

  virtual void getPage(size_t page_index, const unsigned char** begin,
                       const unsigned char** end,
                       PagedByteArray::PagePtr* page) const;

 private:
  struct CachedPage {
    size_t index;
    PagedByteArray::PagePtr page;
    const unsigned char* begin;
    const unsigned char* end;
  };

  // Disallow copy construction and assignment.
  CachedPagedByteArray(const CachedPagedByteArray&);
  void operator=(const CachedPagedByteArray&);

  // Gets the index of the page if it is in the cache and returns true, else
  // returns false.
  bool getFromCache(size_t page_index, size_t* cache_index) const;

  mutable std::mutex mutex_;
  const PagedByteArray* paged_byte_array_;
  const size_t cache_size_;
  mutable std::vector<CachedPage> cached_pages_;
};

}  // namespace binary_parse
}  // namespace piex

#endif  // PIEX_BINARY_PARSE_CACHED_PAGED_BYTE_ARRAY_H_
