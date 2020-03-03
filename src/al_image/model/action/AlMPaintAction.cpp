/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMPaintAction.h"
#include "AlMath.h"
#include "AlLogcat.h"

#define TAG "AlMPaintAction"

AlMPaintAction::AlMPaintAction() : AlAbsMFilterAction(TYPE_MOSAIC) {
//    paint(AlVec2(-0.5f, 0.0f));
//    paint(AlVec2(0.0f, 0.5f));
//    paint(AlVec2(0.5f, 0.0f));
//    paint(AlVec2(0.0f, -0.5f));
//    paint(AlVec2(-0.75f, 0.5f));
//    paint(AlVec2(0.0f, 0.75f));
//    paint(AlVec2(0.75f, -0.5f));
//    int count = 3 + 2 * 8;
//    for (int i = 0; i < count; ++i) {
//        AlVec2 point = AlVec2(AlMath::random(0, 100) / 50.0f - 1.0f,
//                              AlMath::random(0, 100) / 50.0f - 1.0f);
//        paint(point);
//    }
//    newPath();
}

AlMPaintAction::~AlMPaintAction() {
    _original.clear();
    auto itr = path.begin();
    while (path.end() != itr) {
        delete *itr;
        ++itr;
    }
    path.clear();
};

void AlMPaintAction::newPath() {
    std::lock_guard<std::mutex> guard(mtx);
    _original.emplace_back(AlVec2(MAXFLOAT, MAXFLOAT));
    AlLogI(TAG, "%d", _original.size());
}

void AlMPaintAction::paint(const AlVec2 &pointF) {
    std::lock_guard<std::mutex> guard(mtx);
    _original.emplace_back(pointF);
    size_t size = _original.size();
    if (MAXFLOAT == _original[size - 2].x && MAXFLOAT == _original[size - 2].y) {
        splitIndex = size - 1;
    }
    int32_t ss = size - splitIndex - 3;
    if (ss == 0 || (ss > 0 && 0 == ss % 2)) {
        AlVec2 p0 = _original[size - 3];
        AlVec2 p1 = _original[size - 2];
        AlVec2 p2 = _original[size - 1];
        auto *curve = AlBezierCurve::create(p0, p1, p2);
        if (curve->empty()) {
            _original.erase(_original.end() - 2);
            delete curve;
            return;
        }
        path.emplace_back(curve);
        AlLogI(TAG, "paint: (%f, %f),(%f, %f),(%f, %f)", p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
    }
}

HwResult AlMPaintAction::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    return Hw::SUCCESS;
}

void AlMPaintAction::getPath(std::vector<float> &path) {
    for (AlBezierCurve *c:this->path) {
        c->getPath(path);
    }
}

void AlMPaintAction::getDiffPath(std::vector<float> &path) {
    std::lock_guard<std::mutex> guard(mtx);
    size_t size = this->path.size();
    for (; cursor < size; ++cursor) {
        this->path[cursor]->getPath(path);
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
