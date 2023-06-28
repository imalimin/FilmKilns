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
// The cache layer works as follows:
// The cache is implemented as a vector (of size 'cache_size') of shared
// pointers to pages recently used. The least recently used page is stored
// at the begining of the vector, the most recent at the end.

#include "src/binary_parse/cached_paged_byte_array.h"

#include <cstddef>

namespace piex {
namespace binary_parse {

CachedPagedByteArray::CachedPagedByteArray(
    const PagedByteArray* paged_byte_array, size_t cache_size)
    : paged_byte_array_(paged_byte_array), cache_size_(cache_size) {}

void CachedPagedByteArray::getPage(size_t page_index,
                                   const unsigned char** begin,
                                   const unsigned char** end,
                                   PagedByteArray::PagePtr* page) const {
  std::lock_guard<std::mutex> lock(mutex_);
  size_t cache_index;
  if (getFromCache(page_index, &cache_index)) {
    // Cache hit, retrieve the page from the cache.
    *begin = cached_pages_[cache_index].begin;
    *end = cached_pages_[cache_index].end;
    *page = cached_pages_[cache_index].page;

    // Remove the page to insert it at the end of the cache later.
    cached_pages_.erase(cached_pages_.begin() +
                        static_cast<std::ptrdiff_t>(cache_index));
  } else {
    // Cache miss, ask PagedByteArray to load the page.
    paged_byte_array_->getPage(page_index, begin, end, page);

    // If the cache is full, remove the first (least recently used) page.
    if (cached_pages_.size() >= cache_size_) {
      cached_pages_.erase(cached_pages_.begin());
    }
  }

  // Cache the most recently used page to the end of the vector.
  CachedPage cache_page;
  cache_page.index = page_index;
  cache_page.page = *page;
  cache_page.begin = *begin;
  cache_page.end = *end;
  cached_pages_.push_back(cache_page);
}

bool CachedPagedByteArray::getFromCache(size_t page_index,
                                        size_t* cache_index) const {
  for (size_t i = 0; i < cached_pages_.size(); ++i) {
    if (cached_pages_[i].index == page_index) {
      *cache_index = i;
      return true;
    }
  }
  return false;
}

}  // namespace binary_parse
}  // namespace piex
