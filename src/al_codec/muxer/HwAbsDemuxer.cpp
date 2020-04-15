/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsDemuxer.h"

HwAbsDemuxer::HwAbsDemuxer() : Object() {

}

HwAbsDemuxer::~HwAbsDemuxer() {

}

HwResult HwAbsDemuxer::configure(string filePath, string type) {
    this->filePath = filePath;
    this->type = type;
    return Hw::SUCCESS;
}