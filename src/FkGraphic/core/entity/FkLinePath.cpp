/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-18 13:15:10
*/

#include "FkLinePath.h"
#include "FkMath.h"

FK_IMPL_CLASS_TYPE(FkLinePath, FkPath)

FkLinePath::FkLinePath(double _avgDistance, float _pixelsOfSensitivity)
        : FkPath(), avgDistance(_avgDistance), pixelsOfSensitivity(_pixelsOfSensitivity) {
}

FkLinePath::FkLinePath(const FkLinePath &o) : FkPath(o) {
    for (auto &point: o.src) {
        src.emplace_back(point);
    }
    for (auto &point: o.points) {
        points.emplace_back(point);
    }
}

FkLinePath::~FkLinePath() {

}

void FkLinePath::addPoint(FkDoubleVec2 &point) {
    if (!src.empty() && FkMath::distance(src[src.size() - 1], point) < pixelsOfSensitivity) {
        return;
    }
    src.emplace_back(point);
    if (src.size() >= 2) {
        auto &start = src[src.size() - 2];
        auto &end = src[src.size() - 1];
        auto distance = FkMath::distance(start, end);
        int count = distance / avgDistance;
        points.emplace_back(start);
        for (int j = 1; j < count; ++j) {
            float t = j / (float) count;
            FkDoubleVec2 p(start.x + t * (end.x - start.x),
                           start.y + t * (end.y - start.y));
            points.emplace_back(p);
        }
        points.emplace_back(end);
    }
}

size_t FkLinePath::readPoints(std::vector<FkDoubleVec2> &_points) {
    int i = std::max(0, readPos);
    for (; i < points.size(); ++i) {
        _points.emplace_back(points[i]);
    }
    readPos = i;
    return _points.size();
}

size_t FkLinePath::getPoints(std::vector<FkDoubleVec2> &_points) {
    for (auto &point : points) {
        _points.emplace_back(point);
    }
    return _points.size();
}

size_t FkLinePath::size() {
    return points.size() - readPos;
}

void FkLinePath::finish() {
}