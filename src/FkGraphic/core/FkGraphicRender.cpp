/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicRender.h"
#include <utility>

std::shared_ptr<FkGraphicRender> FkGraphicRender::with(std::shared_ptr<FkGraphicProgram> program) {
    auto ptr = new FkGraphicRender();
    std::shared_ptr<FkGraphicRender> render(ptr);
    render->program = std::move(program);
    return render;
}

FkGraphicRender::FkGraphicRender() : FkObject() {
    FK_MARK_SUPER
}

FkGraphicRender::FkGraphicRender(const FkGraphicRender &o) : FkObject(o) {
    FK_MARK_SUPER
}

FkGraphicRender::~FkGraphicRender() {

}

std::shared_ptr<FkGraphicRender> FkGraphicRender::enableSwapBuffers(bool enable) {
    this->_enableSwapBuffers = enable;
    return shared_from_this();
}

FkResult FkGraphicRender::render() {
    context->makeCurrent();
    glViewport(0, 0, size.getWidth(), size.getHeight());
    glClearColor(color.fRed(), color.fGreen(), color.fBlue(), color.fAlpha());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    size_t countOfPosByte = position->countVertex * position->countPerVertex * sizeof(float);
    size_t countOfCoordByte = coordinate->countVertex * coordinate->countPerVertex * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, countOfPosByte, position->data);
    glBufferSubData(GL_ARRAY_BUFFER, countOfPosByte, countOfCoordByte, coordinate->data);

    program->bind();
    program->addValue(srcTex);
    program->addValue(position);
    program->addValue(coordinate);

    FK_GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, position->countVertex))

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    program->clear();
    program->unbind();
    if (_enableSwapBuffers) {
        context->swapBuffers();
    }
    glFlush();
    return FK_FAIL;
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setContext(
        std::shared_ptr<FkGraphicContext> context) {
    this->context = context;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setViewport(int x, int y, int width, int height) {
    size.set(width, height);
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setColor(FkColor color) {
    this->color = color;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setVertexBuffer(uint32_t vbo) {
    this->vbo = vbo;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setSrcTexture(
        int index, std::shared_ptr<FkGraphicTexture> tex) {
    srcTex = std::make_shared<FkTexValue>();
    srcTex->index = index;
    srcTex->tex = tex;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setPosition(
        size_t countVertex, size_t countPerVertex, size_t offset, void *data) {
    position = std::make_shared<FkPositionValue>();
    position->countVertex = countVertex;
    position->countPerVertex = countPerVertex;
    position->offset = offset;
    position->data = data;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setCoordinate(
        size_t countVertex, size_t countPerVertex, size_t offset, void *data) {
    coordinate = std::make_shared<FkCoordinateValue>();
    coordinate->countVertex = countVertex;
    coordinate->countPerVertex = countPerVertex;
    coordinate->offset = offset;
    coordinate->data = data;
    return shared_from_this();
}
