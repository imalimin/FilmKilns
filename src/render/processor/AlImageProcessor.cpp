/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageProcessor.h"
#include "HwBitmapFactory.h"

AlImageProcessor::AlImageProcessor() : HwAbsProcessor("AlImageProcessor") {

}

AlImageProcessor::~AlImageProcessor() {

}

void AlImageProcessor::setCanvas(int32_t w, int32_t h, int32_t color) {
}

HwResult AlImageProcessor::addLayer(const char *path) {
    auto *bitmap = HwBitmapFactory::decodeFile(std::string(path));
    delete bitmap;
    return Hw::SUCCESS;
}
