/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMosaicFilter.h"
#include "HwProgram.h"

constexpr int SIZE_POINT = 10;

AlMosaicFilter::AlMosaicFilter() : HwAbsFilter() {
    path = new float[SIZE_POINT * 2];
}

AlMosaicFilter::~AlMosaicFilter() {
    delete[] path;
};

AlAbsGLProgram *AlMosaicFilter::createProgram() {
    string vertex("attribute vec4 aPosition;\n"
                  "attribute vec4 aTextureCoord;\n"
                  "varying vec2 vTextureCoord;\n"
                  "void main() {\n"
                  "    gl_Position = aPosition;\n"
                  "    vTextureCoord = aTextureCoord.xy;\n"
                  "}");
    string fragment("precision mediump float;\n"
                    "varying mediump vec2 vTextureCoord;\n"
                    "uniform sampler2D uTexture;\n"
                    "uniform vec2 radius;\n"
                    "uniform float paintRadius;\n"
                    "uniform vec2 paths[10];\n"
                    "bool onPath(vec2 point) {\n"
                    "    for(int i = 0; i < 10; i++) {\n"
                    "        if(distance(point, paths[i]) <= paintRadius){"
                    "            return true;"
                    "        }"
                    "    }"
                    "    return false;\n"
                    "}\n"
                    "void main() {\n"
                    "    vec2 point = vec2(vTextureCoord.x, vTextureCoord.y);\n"
                    "    if(onPath(point)) {\n"
                    "        vec2 diameter = vec2(radius.x * 2.0, radius.y * 2.0);\n"
                    "        vec2 eachXY = vec2(floor(vTextureCoord.x / diameter.x), floor(vTextureCoord.y / diameter.y));\n"
                    "        vec2 totalXY = vec2(floor(1.0 / diameter.x), floor(1.0 / diameter.y));\n"
                    "        point.x = eachXY.x / totalXY.x;\n"
                    "        point.y = eachXY.y / totalXY.y;\n"
                    "    }"
                    "    vec4 color = texture2D(uTexture, point);\n"
                    "    gl_FragColor = color;\n"
                    "}");
    HwProgram *program = HwProgram::create(&vertex, &fragment);
    if (program) {
        uRadius = program->getUniformLocation("radius");
        uPaintRadius = program->getUniformLocation("paintRadius");
        uPaths = program->getUniformLocation("paths");
    }
    return program;
}

void AlMosaicFilter::drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    program->bind();
    float size = dest->getWidth() * 0.05f / 2.0f;
    radius.x = size / (float) dest->getWidth();
    radius.y = size / (float) dest->getHeight();
    program->setUniform2fv(uRadius, radius);
    program->setUniform1f(uPaintRadius, radius.x * 2);
    program->setUniform2fv(uPaths, SIZE_POINT, path);
}

void AlMosaicFilter::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::draw(src, dest);
}

void AlMosaicFilter::updatePath(std::vector<AlPointF> *vec) {
    size_t size = vec->size();
    for (int i = 0; i < SIZE_POINT; ++i) {
        if (i < size) {
            AlPointF p = (*vec)[i];
            path[i * 2 + 0] = (p.x + 1) / 2.0f;
            path[i * 2 + 1] = 1 - (p.y + 1) / 2.0f;
        } else {
            path[i * 2 + 0] = -1000;
            path[i * 2 + 1] = -1000;
        }
    }
}
