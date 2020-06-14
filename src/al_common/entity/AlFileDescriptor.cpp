/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFileDescriptor.h"

AlFileDescriptor::AlFileDescriptor(std::string path)
        : AlAbsInputDescriptor(kType::FILE),
          _path(path) {

}

AlFileDescriptor::AlFileDescriptor(const AlFileDescriptor &o)
        : AlAbsInputDescriptor(o), _path(o._path) {

}

AlFileDescriptor::~AlFileDescriptor() {

}