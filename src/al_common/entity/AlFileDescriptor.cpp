/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFileDescriptor.h"

const AlFileDescriptor AlFileDescriptor::EMPTY = AlFileDescriptor("/empty");

AlFileDescriptor::AlFileDescriptor(std::string path)
        : AlAbsInputDescriptor(kType::FILE),
          _path(path) {

}

AlFileDescriptor::AlFileDescriptor(const AlFileDescriptor &o)
        : AlAbsInputDescriptor(o), _path(o._path) {

}

AlFileDescriptor::~AlFileDescriptor() {

}

std::string AlFileDescriptor::path() {
    return _path;
}

bool AlFileDescriptor::operator==(const AlFileDescriptor &o) {
    return o._path == this->_path;
}

bool AlFileDescriptor::equals(const Object &o) {
    auto &o1 = (AlFileDescriptor &) o;
    return o1._path == this->_path;
}
