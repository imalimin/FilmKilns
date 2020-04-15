/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPaintFilter.h"
#include "AlPointProgram.h"
#include "AlMath.h"
#include "AlTexManager.h"
#include "AlPaintRoundFilter.h"
#include "Logcat.h"

constexpr int DIFF_SIZE = 100;
#define TAG "AlPaintFilter"

AlPaintFilter::AlPaintFilter() : HwAbsFilter() {

}

AlPaintFilter::~AlPaintFilter() {
    if (roundTex) {
        AlTexManager::instance()->recycle(&roundTex);
        roundTex = nullptr;
    }
    path.clear();
}

bool AlPaintFilter::prepare() {
    bool ret = HwAbsFilter::prepare();
    if (nullptr == roundTex) {
        AlTexDescription desc;
        desc.size.width = 64;
        desc.size.height = 64;
        roundTex = AlTexManager::instance()->alloc(desc);
        AlPaintRoundFilter *filter = new AlPaintRoundFilter();
        filter->prepare();
        glViewport(0, 0, roundTex->getWidth(), roundTex->getHeight());
        filter->draw(roundTex, roundTex);
        delete filter;
    }
    return ret;
}

void AlPaintFilter::setPath(std::vector<float> *vec, bool clear) {
    if (clear) {
        path.clear();
    }
    if (nullptr == vec || vec->empty()) {
        return;
    }
    for (float it:*vec) {
        path.emplace_back(it);
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
                    "uniform sampler2D uTexture;"
                    "uniform vec4 color;\n"
                    "void main() {\n"
                    "    vec4 c = texture2D(uTexture, vec2(gl_PointCoord.x, 1.0 - gl_PointCoord.y));\n"
                    "    gl_FragColor = color * c;\n"
                    "}");
    AlAbsGLProgram *program = AlPointProgram::create(&vertex, &fragment);
    uSize = program->getUniformLocation("size");
    uColor = program->getUniformLocation("color");
    return program;
}

void AlPaintFilter::drawFirst(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawFirst(program, src, dest);
    program->bind();
    Logcat::i(TAG, "%s(%d): %d", __FUNCTION__, __LINE__, path.size() / 2);
    dynamic_cast<AlPointProgram *>(program)->setVertex(path, 2, path.size() / 2);
    float color[4];
    color[0] = this->color.rf();
    color[1] = this->color.gf();
    color[2] = this->color.bf();
    color[3] = 1.0f - this->color.af();
    program->setUniform4fv(uColor, 1, color);
    program->setUniform1f(uSize, paintSize * dest->getWidth());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
}

void AlPaintFilter::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    if (roundTex) {
        HwAbsFilter::draw(roundTex, dest);
    }
}

void AlPaintFilter::drawEnd(AlAbsGLProgram *program, HwAbsTexture *src, HwAbsTexture *dest) {
    HwAbsFilter::drawEnd(program, src, dest);
    glDisable(GL_BLEND);
}
