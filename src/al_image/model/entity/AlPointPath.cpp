/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPointPath.h"
#include "AlMath.h"

static float lengthWithT(float t, float a, float b, float c) {
    if (a < 0.00001f) {
        return 0.0f;
    }

    float temp1 = sqrtf(c + t * (b + a * t));
    float temp2 = (2 * a * t * temp1 + b * (temp1 - sqrtf(c)));
    float temp3 = log(std::abs(b + 2 * sqrtf(a) * sqrtf(c) + 0.0001f));
    float temp4 = log(std::abs(b + 2 * a * t + 2 * sqrtf(a) * temp1) + 0.0001f);
    float temp5 = 2 * sqrtf(a) * temp2;
    float temp6 = (b * b - 4 * a * c) * (temp3 - temp4);

    return (temp5 + temp6) / (8 * powf(a, 1.5f));
}

static float speedWithT(float t, float a, float b, float c) {
    return sqrtf(std::max<float>(a * pow(t, 2.0) + b * t + c, 0));
}

static float tWithT(float t, float len, float a, float b, float c) {
    float t1 = t;
    float t2;

    while (true) {
        float speed = speedWithT(t1, a, b, c);
        if (speed < 0.0001f) {
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

static void bezier(AlVec2 from, AlVec2 to, AlVec2 control,
                   float paintSize, std::vector<float> &points) {
    AlVec2 P0 = from;
    // 如果 control 是 from 和 to 的中点，则将 control 设置为和 from 重合
    bool isCenter = ((from + to) / 2.0f - control) < AlVec2(0.0001f, 0.0001f);
    AlVec2 P1 = isCenter ? from : control;
    AlVec2 P2 = to;

    float ax = P0.x - 2 * P1.x + P2.x;
    float ay = P0.y - 2 * P1.y + P2.y;
    float bx = 2 * P1.x - 2 * P0.x;
    float by = 2 * P1.y - 2 * P0.y;

    float a = 4 * (ax * ax + ay * ay);
    float b = 4 * (ax * bx + ay * by);
    float c = bx * bx + by * by;

    float totalLength = lengthWithT(1, a, b, c);  // 整条曲线的长度
    float pointsPerLength = 5.0 / paintSize;  // 用点的尺寸计算出，单位长度需要多少个点
    int count = std::max<float>(1, std::ceil(pointsPerLength * totalLength));  // 曲线应该生成的点数

    for (int i = 0; i <= count; ++i) {
        float t = i * 1.0f / count;
        float length = t * totalLength;
        t = tWithT(t, length, a, b, c);
        // 根据 t 求出坐标
        float x = (1 - t) * (1 - t) * P0.x + 2 * (1 - t) * t * P1.x + t * t * P2.x;
        float y = (1 - t) * (1 - t) * P0.y + 2 * (1 - t) * t * P1.y + t * t * P2.y;
        points.emplace_back(x);
        points.emplace_back(y);
    }
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
        std::vector<float> points;
        bezier(p0, p2, p1, 0.01f, points);
        for (float it:points) {
            _path.emplace_back(it);
        }
    }
}

std::vector<float> *AlPointPath::path() {
    return &_path;
}
