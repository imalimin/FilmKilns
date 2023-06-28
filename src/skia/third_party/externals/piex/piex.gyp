# Copyright 2015 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
{
'includes': ['internal_include_do_not_delete.gypi'],
'targets': [{
  'target_name': 'piex',
  'type': 'static_library',
  'sources': [
    'src/piex.cc',
    'src/tiff_parser.cc',
  ],
  'variables': {
    'headers': [
      'src/piex.h',
      'src/piex_types.h',
      'src/tiff_parser.h',
    ],
  },
  'include_dirs': ['.'],
  'cflags': [
    '-Wsign-compare',
    '-Wsign-conversion',
    '-Wunused-parameter',
  ],
  'dependencies': [
    'binary_parse',
    'image_type_recognition',
    'tiff_directory',
  ],
}, {
  'target_name': 'binary_parse',
  'type': 'static_library',
  'sources': [
      'src/binary_parse/cached_paged_byte_array.cc',
      'src/binary_parse/range_checked_byte_ptr.cc',
  ],
  'variables': {
    'headers': [
      'src/binary_parse/cached_paged_byte_array.h',
      'src/binary_parse/range_checked_byte_ptr.h',
    ],
  },
  'include_dirs': ['.'],
  'cflags': [
    '-Wsign-compare',
    '-Wsign-conversion',
    '-Wunused-parameter',
  ],
}, {
  'target_name': 'image_type_recognition',
  'type': 'static_library',
  'sources': [
    'src/image_type_recognition/image_type_recognition_lite.cc',
  ],
  'variables': {
    'headers': ['src/image_type_recognition/image_type_recognition_lite.h'],
  },
  'include_dirs': ['.'],
  'cflags': [
    '-Wsign-compare',
    '-Wsign-conversion',
    '-Wunused-parameter',
  ],
  'dependencies': ['binary_parse'],
}, {
  'target_name': 'tiff_directory',
  'type': 'static_library',
  'cflags': [
    '-Wsign-compare',
    '-Wsign-conversion',
    '-Wunused-parameter',
  ],
  'sources': [
    'src/tiff_directory/tiff_directory.cc',
  ],
  'variables': {
    'headers': ['src/tiff_directory/tiff_directory.h'],
  },
  'include_dirs': ['.'],
  'dependencies': ['binary_parse'],
}],
}
