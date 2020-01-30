/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPointPath.h"
#include "AlMath.h"
#include "AlLogcat.h"

#define TAG "AlPointPath"

/// p = (1 - t) ^ 2 * p0 + 2 * t * (1 - t) * p1 + t ^ 2 * p2
static AlVec2 bezier_point(AlVec2 p0, AlVec2 p1, AlVec2 p2, AlVec2 p3, double t) {
    double it = 1 - t;
    return AlVec2(
            it * it * it * p0.x + 3 * it * it * t * p1.x + 3 * it * t * t * p2.x + t * t * t * p3.x,
            it * it * it * p0.y + 3 * it * it * t * p1.y + 3 * it * t * t * p2.y +
            t * t * t * p3.y);
}

static AlVec2 bezier_speed_point(AlVec2 p0, AlVec2 p1, AlVec2 p2, AlVec2 p3, double t) {
    double it = 1 - t;
    return AlVec2(
            -3 * p0.x * it * it + 3 * p1.x * it * it - 6 * p1.x * it * t + 6 * p2.x * it * t -
            3 * p2.x * t * t + 3 * p3.x * t * t,
            -3 * p0.y * it * it + 3 * p1.y * it * it - 6 * p1.y * it * t + 6 * p2.y * it * t -
            3 * p2.y * t * t + 3 * p3.y * t * t);
}

static double bezier_speed(AlVec2 p0, AlVec2 p1, AlVec2 p2, AlVec2 p3, double t) {
    AlVec2 speed = bezier_speed_point(p0, p1, p2, p3, t);
    return sqrt(speed.x * speed.x + speed.y * speed.y);
}

static double bezier_length(AlVec2 p0, AlVec2 p1, AlVec2 p2, AlVec2 p3, double t) {
#define TOTAL_SIMPSON_STEP    (10000)
    int stepCounts = (int) (TOTAL_SIMPSON_STEP * t);
    if (stepCounts & 1) stepCounts++;    //偶数
    if (stepCounts == 0) return 0.0;

    int halfCounts = stepCounts / 2;
    double sum1 = 0.0, sum2 = 0.0;
    double dStep = t / stepCounts;

    for (int i = 0; i < halfCounts; i++) {
        sum1 += bezier_speed(p0, p1, p2, p3, (2 * i + 1) * dStep);
    }

    for (int i = 1; i < halfCounts; i++) {
        sum2 += bezier_speed(p0, p1, p2, p3, (2 * i) * dStep);
    }

    return (bezier_speed(p0, p1, p2, p3, 0.0) + bezier_speed(p0, p1, p2, p3, 1.0) + 2 * sum2 +
            4 * sum1) * dStep / 3.0;
}

static double bezier_even(AlVec2 p0, AlVec2 p1, AlVec2 p2, AlVec2 p3, double t) {
    double len = t * bezier_length(p0, p1, p2, p3, 1.0);
    double t1 = t, t2;

    do {
        t2 = t1 - (bezier_length(p0, p1, p2, p3, t1) - len) / bezier_length(p0, p1, p2, p3, t1);
        if (abs(t1 - t2) < 0.0000001f) break;
        t1 = t2;
    } while (true);
    return t2;
}

static void bezierV2(AlVec2 p0, AlVec2 p1, AlVec2 p2, AlVec2 p3,
                     float paintSize, std::vector<float> &points) {

}

AlPointPath::AlPointPath() : Object() {
}

AlPointPath::AlPointPath(const AlPointPath &o) : Object(), original(o.original), _path(o._path) {

}

AlPointPath::~AlPointPath() {
    _path.clear();
    original.clear();
}

void AlPointPath::paintTo(const AlVec2 &p) {
    if (original.empty()) {
        original.emplace_back(p);
        return;
    }
    original.emplace_back(p);
    size_t size = original.size();
    if (size >= 3 && 0 == size % 2) {
        AlVec2 p0 = original[size - 3];
        AlVec2 p1 = original[size - 2];
        AlVec2 p2 = original[size - 1];
        AlLogI(TAG, "(%f,%f),(%f,%f),(%f,%f)",
               p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
        AlBezierCurve *curve = AlBezierCurve::create(p0, p1, p2);
        std::vector<float> points;
        curve->getPath(points);
        for (float it:points) {
            _path.emplace_back(it);
        }
        delete curve;
    }
}

std::vector<float> *AlPointPath::path() {
    return &_path;
}
