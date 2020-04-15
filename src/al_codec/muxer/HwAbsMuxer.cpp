/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsMuxer.h"

const string HwAbsMuxer::TYPE_MP4 = "MP4";
const int32_t HwAbsMuxer::TRACK_NONE = -1;

HwAbsMuxer::HwAbsMuxer() : Object() {

}

HwAbsMuxer::~HwAbsMuxer() {

}

HwResult HwAbsMuxer::configure(string filePath, string type) {
    this->filePath = filePath;
    this->type = type;
    return Hw::SUCCESS;
}