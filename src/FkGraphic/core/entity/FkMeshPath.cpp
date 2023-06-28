/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-10 12:58:54
*/

#include "FkMeshPath.h"
#include "FkMath.h"
#include <cmath>

FK_IMPL_CLASS_TYPE(FkMeshPath, FkPath)

FkMeshPath::FkMeshPath(std::vector<FkDoubleVec2> &meshPoints)
        : FkPath(), strokeWidth(1) {
    for (auto &p: meshPoints) {
        this->meshPoints.emplace_back(p);
    }
}

FkMeshPath::FkMeshPath(std::shared_ptr<FkPath> &parent, float _strokeWidth)
        : FkPath(), parent(parent), strokeWidth(_strokeWidth) {
}

FkMeshPath::FkMeshPath(const FkMeshPath &o)
        : FkPath(o), parent(o.parent), strokeWidth(o.strokeWidth) {

}

FkMeshPath::~FkMeshPath() {

}

void FkMeshPath::addPoint(FkDoubleVec2 &point) {
    parent->addPoint(point);

    FkDoubleVec2 front(0, 0);
    FkDoubleVec2 current(0, 0);
    FkDoubleVec2 next(0, 0);

    FkDoubleVec2 dir(0, 0);
    FkDoubleVec2 dir1(0, 0);
    FkDoubleVec2 dir2(0, 0);

    std::vector<FkDoubleVec2> points;
    auto count = parent->readPoints(points);
    for (int i = 0; i < count; ++i) {
        //第一个
        if (i == 0) {
            current = FkDoubleVec2(points[i].x, points[i].y);
            front = FkDoubleVec2(points[i].x, points[i].y);
            next = FkDoubleVec2(points[i + 1].x, points[i + 1].y);
        } else if (i == (count - 1)) {//最后一个
            current = FkDoubleVec2(points[i].x, points[i].y);
            front = FkDoubleVec2(points[i - 1].x, points[i - 1].y);
            next = FkDoubleVec2(points[i].x, points[i].y);
        } else {
            current = FkDoubleVec2(points[i].x, points[i].y);
            front = FkDoubleVec2(points[i - 1].x, points[i - 1].y);
            next = FkDoubleVec2(points[i + 1].x, points[i + 1].y);
        }

        if (abs(next.x - current.x) <= 0.000001 && abs(next.y - current.y) <= 0.000001) {
            dir = (current - front).normalize();
        } else if (abs(front.x - current.x) <= 0.000001 && abs(front.y - current.y) <= 0.000001) {
            dir = (next - current).normalize();
        } else {
            dir1 = (current - front).normalize();
            dir2 = (next - current).normalize();
            dir = (dir1 + dir2).normalize();
        }

        FkDoubleVec2 normal = FkDoubleVec2(-dir.y, dir.x);
        if (i == 0) {
            FkDoubleVec2 tmp = current + normal * strokeWidth * 1 * 1 / 2;
            meshPoints.emplace_back(FkDoubleVec2(tmp.x, tmp.y));
            tmp = current + normal * strokeWidth * -1 * 1 / 2;
            meshPoints.emplace_back(FkDoubleVec2(tmp.x, tmp.y));
            continue;
        } else {
            FkDoubleVec2 vp = current - front;
            float len = std::sqrt(vp.x * vp.x + vp.y * vp.y);

            //计算拐角点
            FkDoubleVec2 tmp = current + normal * strokeWidth * 1 * 1 / 2;
            meshPoints.emplace_back(FkDoubleVec2(tmp.x, tmp.y));
            tmp = current + normal * strokeWidth * -1 * 1 / 2;
            meshPoints.emplace_back(FkDoubleVec2(tmp.x, tmp.y));
        }
    }
}

size_t FkMeshPath::readPoints(std::vector<FkDoubleVec2> &_points) {
    int i = std::max(0, readPos);
    for (; i < meshPoints.size(); ++i) {
        _points.emplace_back(meshPoints[i]);
    }
    readPos = std::max(0, i - 2);;
    return _points.size();
}

size_t FkMeshPath::size() {
    return meshPoints.size() - readPos;
}

size_t FkMeshPath::getPoints(std::vector<FkDoubleVec2> &_points) {
    for (auto &point: meshPoints) {
        _points.emplace_back(point);
    }
    return _points.size();
}

void FkMeshPath::finish() {
    parent->finish();
}
