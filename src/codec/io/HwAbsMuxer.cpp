/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsMuxer.h"

const string HwAbsMuxer::TYPE_MP4 = "MP4";

HwAbsMuxer::HwAbsMuxer(string filePath, string type) : Object(), filePath(filePath), type(type) {

}

HwAbsMuxer::~HwAbsMuxer() {

}