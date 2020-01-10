/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPaintFilter.h"
#include "AlPointProgram.h"
#include "AlMath.h"
#include "Logcat.h"

constexpr int DIFF_SIZE = 100;
#define TAG "AlPaintFilter"

AlPaintFilter::AlPaintFilter() : HwAbsFilter() {

}

AlPaintFilter::~AlPaintFilter() {
    path.clear();
}

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

static void bezier(AlPointF from, AlPointF to, AlPointF control,
                   float paintSize, std::vector<float> &points) {
    AlPointF P0 = from;
    // 如果 control 是 from 和 to 的中点，则将 control 设置为和 from 重合
    bool isCenter = ((from + to) / 2.0f - control) < AlVec2(0.0001f, 0.0001f);
    AlPointF P1 = isCenter ? from : control;
    AlPointF P2 = to;

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

void AlPaintFilter::setPath(std::vector<float> *vec) {
    if (nullptr == vec || vec->empty()) {
        return;
    }
    size_t size = vec->size() / 2;
    if (size < 3) {
        return;
    }
    path.clear();
    for (int i = 0; i < size - 3; ++i) {
        AlPointF p0(vec->at(i * 2), vec->at(i * 2 + 1));
        AlPointF p1(vec->at((i + 1) * 2), vec->at((i + 1) * 2 + 1));
        AlPointF p2(vec->at((i + 2) * 2), vec->at((i + 2) * 2 + 1));
        std::vector<float> points;
        bezier(p0, p2, p1, paintSize, points);
        for (float it:points) {
            path.emplace_back(it);
        }
    }
}

void AlPaintFilter::setPaintSize(float size) {
    this->paintSize = size;
}

void AlPaintFilter::setColor(AlColor color) {
    this->color = color;
}

AlAbsGLProgram *AlPaintFilter::createProgram() {
    string vertex("attribute vec4 aPosition;\n"
                  "uniform float size;\n"
                  "void main() {\n"
                  "    gl_Position = aPosition;\n"
                  "    gl_PointSize = size;\n"
                  "}");
    string fragment("precision mediump float;\n"
                    "uniform vec4 color;\n"
                    "void main() {\n"
                    "    gl_FragColor = color;\n"
                    "}");
    AlAbsGLProgram *program = AlPointProgram::create(&vertex, &fragment);
    uSize = program->getUniformLocation("size");
    uColor = program->getUniformLocation("color");
    return program;
}

void AlPaintFilter::drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    program->bind();
    if (!path.empty()) {
        dynamic_cast<AlPointProgram *>(program)->setVertex(path, 2, path.size() / 2);
    }
    float color[4];
    color[0] = this->color.rf();
    color[1] = this->color.gf();
    color[2] = this->color.bf();
    color[3] = 1.0f - this->color.af();
    program->setUniform4fv(uColor, 1, color);
    program->setUniform1f(uSize, paintSize * dest->getWidth());
}

void AlPaintFilter::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::draw(src, dest);
}
