/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMMosaicAction.h"
#include "Logcat.h"

#define TAG "AlMMosaicAction"

AlMMosaicAction::AlMMosaicAction() : AlAbsMAction(TYPE_MOSAIC) {

}

AlMMosaicAction::~AlMMosaicAction() = default;

void AlMMosaicAction::addPoint(const AlPointF &pointF) {
    this->points.emplace_back(pointF);
    Logcat::i(TAG, "%s(%d) addPoint %d", __FUNCTION__, __LINE__, points.size());
}

HwResult AlMMosaicAction::measure(AlImgLayerDescription &layer,
                                  AlImageLayerDrawModel *description) {
    description->mosaicPath = &points;
    return Hw::SUCCESS;
}

std::vector<AlPointF> *AlMMosaicAction::getPath() {
    return &points;
}

HwResult AlMMosaicAction::fromElement(AlElement *element) {
    return Hw::SUCCESS;
}

HwResult AlMMosaicAction::toElement(AlElement **element) {
    return Hw::SUCCESS;
}
