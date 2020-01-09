/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMMosaicAction.h"
#include "Logcat.h"

#define TAG "AlMMosaicAction"

AlMMosaicAction::AlMMosaicAction() : AlAbsMFilterAction(TYPE_MOSAIC) {

}

AlMMosaicAction::~AlMMosaicAction() {
    size_t size = path.size();
    for (int i = 0; i < size; ++i) {
        AlVec2 *vec = path[i];
        delete vec;
    }
    path.clear();
};

void AlMMosaicAction::addPoint(const AlVec2 &pointF) {
    this->path.emplace_back(new AlVec2(pointF));
    Logcat::i(TAG, "%s(%d) addPoint %d", __FUNCTION__, __LINE__, path.size());
}

HwResult AlMMosaicAction::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    return Hw::SUCCESS;
}

std::vector<AlVec2 *> *AlMMosaicAction::getPath() {
    return &path;
}

HwResult AlMMosaicAction::fromElement(AlElement *element) {
    return Hw::SUCCESS;
}

HwResult AlMMosaicAction::toElement(AlElement **element) {
    return Hw::SUCCESS;
}
