/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMPaintAction.h"
#include "Logcat.h"

#define TAG "AlMMosaicAction"

AlMPaintAction::AlMPaintAction() : AlAbsMFilterAction(TYPE_MOSAIC) {
    newPath();
}

AlMPaintAction::~AlMPaintAction() {
    path.clear();
};

void AlMPaintAction::newPath() {
    std::lock_guard<std::mutex> guard(mtx);
    path.emplace_back(new AlPointPath());
}

void AlMPaintAction::paint(const AlVec2 &pointF) {
    if (!path.empty()) {
        std::lock_guard<std::mutex> guard(mtx);
        path[path.size() - 1]->paintTo(pointF);
    }
    Logcat::i(TAG, "%s(%d) addPoint %d", __FUNCTION__, __LINE__, path.size());
}

HwResult AlMPaintAction::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    return Hw::SUCCESS;
}

void AlMPaintAction::getPath(std::vector<float> &path) {
    for (AlPointPath *p:this->path) {
        auto *temp = p->path();
        for (float it:*temp) {
            path.emplace_back(it);
        }
    }
}

void AlMPaintAction::getDiffPath(std::vector<float> &path) {
    std::lock_guard<std::mutex> guard(mtx);
    size_t size = this->path.size();
    int i = 0, j = 0;
    for (i = row; i < size; ++i) {
        AlPointPath *p = this->path[i];
        size_t len = p->path()->size();
        for (j = (i == row ? col : 0); j < len; ++j) {
            path.emplace_back(p->path()->at(j));
        }
    }
    if (!path.empty()) {
        if (0 != i) {
            i -= 1;
        }
        row = i;
        if (j >= 2) {
            j -= 2;
        }
        col = j;
    }
}

size_t AlMPaintAction::countOfPath() {
    return this->path.size();
}

void AlMPaintAction::setPaintSize(float size) {
    this->paintSize = size;
}

float AlMPaintAction::getPaintSize() {
    return paintSize;
}

void AlMPaintAction::setColor(AlColor color) {
    this->color = color;
}

AlColor AlMPaintAction::getColor() {
    return color;
}

HwResult AlMPaintAction::fromElement(AlElement *element) {
    return Hw::SUCCESS;
}

HwResult AlMPaintAction::toElement(AlElement **element) {
    return Hw::SUCCESS;
}
