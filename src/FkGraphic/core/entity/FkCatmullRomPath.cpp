/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-5-8 0:08:17
*/

#include "FkCatmullRomPath.h"
#include "FkMath.h"

FK_IMPL_CLASS_TYPE(FkCatmullRomPath, FkPath)

FkCatmullRomPath::FkCatmullRomPath(double _avgDistance, float _pixelsOfSensitivity)
        : FkPath(), avgDistance(_avgDistance), pixelsOfSensitivity(_pixelsOfSensitivity) {
    src.emplace_back(FkDoubleVec2(0, 0));
}

FkCatmullRomPath::FkCatmullRomPath(const FkCatmullRomPath &o) : FkPath(o) {
    for (auto &point : o.src) {
        src.emplace_back(point);
    }
    for (auto &point : o.points) {
        points.emplace_back(point);
    }
}

FkCatmullRomPath::~FkCatmullRomPath() {

}

static FkDoubleVec2 calcCatmullRomPoint(FkDoubleVec2 &p0, FkDoubleVec2 &p1, FkDoubleVec2 &p2, FkDoubleVec2 &p3, float t) {
    int x = p0.x * (-0.5 * t * t * t + t * t - 0.5 * t) +
            p1.x * (1.5 * t * t * t - 2.5 * t * t + 1.0) +
            p2.x * (-1.5 * t * t * t + 2.0 * t * t + 0.5 * t) +
            p3.x * (0.5 * t * t * t - 0.5 * t * t);
    int y = p0.y * (-0.5 * t * t * t + t * t - 0.5 * t) +
            p1.y * (1.5 * t * t * t - 2.5 * t * t + 1.0) +
            p2.y * (-1.5 * t * t * t + 2.0 * t * t + 0.5 * t) +
            p3.y * (0.5 * t * t * t - 0.5 * t * t);
    return FkDoubleVec2(x, y);
}

void FkCatmullRomPath::addPoint(FkDoubleVec2 &point) {
    if (!src.empty() && FkMath::distance(src[src.size() - 1], point) < pixelsOfSensitivity) {
        return;
    }
    src.emplace_back(point);
    if (src.size() == 3) {
        src[0].x = src[1].x * 2 - src[2].x;
        src[0].y = src[1].y * 2 - src[2].y;
    }
    if (src.size() >= 4) {
        auto &start = src[src.size() - 3];
        auto &end = src[src.size() - 2];
        auto distance = FkMath::distance(start, end);
        int count = distance / avgDistance;
        points.emplace_back(start);
        for (int j = 1; j < count; ++j) {
            float t = j / (float) count;
            auto p = calcCatmullRomPoint(src[src.size() - 4], start, end, src[src.size() - 1], t);
            points.emplace_back(p);
        }
        points.emplace_back(end);
    }
}

size_t FkCatmullRomPath::readPoints(std::vector<FkDoubleVec2> &_points) {
    int i = std::max(0, readPos);
    for (; i < points.size(); ++i) {
        _points.emplace_back(points[i]);
    }
    readPos = i;
    return _points.size();
}

void FkCatmullRomPath::finish() {
    if (src.size() >= 3) {
        FkDoubleVec2 point(src[src.size() - 1].x * 2 - src[src.size() - 2].x,
                        src[src.size() - 1].y * 2 - src[src.size() - 2].y);
        addPoint(point);
    }
}

size_t FkCatmullRomPath::size() {
    return points.size() - readPos;
}

size_t FkCatmullRomPath::getPoints(std::vector<FkDoubleVec2> &_points) {
    for (auto &point : points) {
        _points.emplace_back(point);
    }
    return _points.size();
}
