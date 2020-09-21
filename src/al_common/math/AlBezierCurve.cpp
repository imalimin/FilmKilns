/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlBezierCurve.h"
#include "AlMath.h"
#include "AlLogcat.h"

#define TAG "AlBezierCurve"

constexpr float DEVIATION = 0.00001F;

static float lengthWithT(float t, float a, float b, float c) {
//    if (a < DEVIATION || b < DEVIATION || c < DEVIATION) {
    if (abs(a) < DEVIATION) {
        return 0.0F;
    }

    float temp1 = sqrtf(c + t * (b + a * t));
    float temp2 = (2 * a * t * temp1 + b * (temp1 - sqrtf(c)));
    float temp3 = log(std::abs(b + 2 * sqrtf(a) * sqrtf(c) + 0.0001f));
    float temp4 = log(std::abs(b + 2 * a * t + 2 * sqrtf(a) * temp1) + 0.0001f);
    float temp5 = 2 * sqrtf(a) * temp2;
    float temp6 = (b * b - 4 * a * c) * (temp3 - temp4);

    float den = 8 * powf(a, 1.5F);
    return 0 == den ? 0.0F : (temp5 + temp6) / den;
}

static float speedWithT(float t, float a, float b, float c) {
    return sqrtf(std::max<float>(a * pow(t, 2.0) + b * t + c, 0));
}

static float tWithT(float t, float len, float a, float b, float c) {
    float t1 = t;
    float t2;

    while (true) {
        float speed = speedWithT(t1, a, b, c);
        if (speed < DEVIATION) {
            t2 = t1;
            break;
        }
        t2 = t1 - (lengthWithT(t1, a, b, c) - len) / speed;
        if (std::abs(t1 - t2) < 0.0001f) {
            break;
        }
        t1 = t2;
    }
    return t2;
}

AlBezierCurve *AlBezierCurve::createEmpty() {
    return create(AlVec2(0, 0), AlVec2(0, 0), AlVec2(0, 0));
}

AlBezierCurve *AlBezierCurve::create(const AlVec2 &p0, const AlVec2 &p1, const AlVec2 &p2) {
    return new AlBezierCurve(p0, p1, p2);
}

AlBezierCurve::AlBezierCurve(const AlVec2 &p0, const AlVec2 &p1, const AlVec2 &p2)
        : Object(), p0(p0), p1(p1), p2(p2) {
    _length = _calculateLength();
}

AlBezierCurve::AlBezierCurve(const AlBezierCurve &o)
        : Object(), p0(o.p0), p1(o.p1), p2(o.p2), _length(o._length) {

}

AlBezierCurve::~AlBezierCurve() {

}

void AlBezierCurve::getPath(vector<float> &path) {
    _bezier(0.01f, path);
    size_t size = path.size();
    float max = 0.0f;
    for (int i = 1; i < size / 2; ++i) {
        float fx = path[(i - 1) * 2];
        float fy = path[(i - 1) * 2 + 1];
        float tx = path[i * 2];
        float ty = path[i * 2 + 1];
        float l = sqrt(pow(fx - tx, 2.0) + pow(fy - ty, 2.0));
        if (l > max) {
            max = l;
        }
    }
    AlLogI(TAG, "size=%d, max length=%f", size / 2, max);
}

bool AlBezierCurve::empty() {
    return _length < 0.001F || (p0 == p1 && p1 == p2);
}

float AlBezierCurve::_calculateLength() {
    // 如果 control 是 from 和 to 的中点，则将 control 设置为和 from 重合
    AlVec2 delta = (p0 + p2) / 2.0f - p1;
    bool isCenter = delta < AlVec2(DEVIATION, DEVIATION)
                    && delta > AlVec2(-DEVIATION, -DEVIATION);
//    bool isCenter = ((from + to) / 2.0f) == control;
    AlVec2 P1 = isCenter ? p0 : p1;

    float ax = p0.x - 2 * P1.x + p2.x;
    float ay = p0.y - 2 * P1.y + p2.y;
    float bx = 2 * P1.x - 2 * p0.x;
    float by = 2 * P1.y - 2 * p0.y;

    a = 4 * (ax * ax + ay * ay);
    b = 4 * (ax * bx + ay * by);
    c = bx * bx + by * by;
    return lengthWithT(1, a, b, c);  // 整条曲线的长度
}

void AlBezierCurve::_bezier(float paintSize, std::vector<float> &points) {
    float pointsPerLength = 5.0F / paintSize;  // 用点的尺寸计算出，单位长度需要多少个点
    int count = std::ceil(std::max<float>(1, pointsPerLength * _length));  // 曲线应该生成的点数

    points.emplace_back(p0.x);
    points.emplace_back(p0.y);
    for (int i = 0; i <= count; ++i) {
        float t = i * 1.0f / count;
        float length = t * _length;
        t = tWithT(t, length, a, b, c);
        // 根据 t 求出坐标
        float x = (1 - t) * (1 - t) * p0.x + 2 * (1 - t) * t * p1.x + t * t * p2.x;
        float y = (1 - t) * (1 - t) * p0.y + 2 * (1 - t) * t * p1.y + t * t * p2.y;
        points.emplace_back(x);
        points.emplace_back(y);
    }
    points.emplace_back(p2.x);
    points.emplace_back(p2.y);
}
