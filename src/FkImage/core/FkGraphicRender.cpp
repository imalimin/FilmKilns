/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicRender.h"

std::shared_ptr<FkGraphicRender> FkGraphicRender::with(std::shared_ptr<FkGraphicProgram> program) {
    auto ptr = new FkGraphicRender();
    std::shared_ptr<FkGraphicRender> render(ptr);
    render->program = program;
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

FkResult FkGraphicRender::render(kRenderMode mode) {
    context->makeCurrent();
    if (_enableBlend) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
    }

    glViewport(0, 0, size.getWidth(), size.getHeight());
    if (nullptr != color) {
        glClearColor(color->fRed(), color->fGreen(), color->fBlue(), color->fAlpha());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    if (!_enableSwapBuffers) {
        FkAssert(nullptr != fbo, FK_FAIL);
        FkAssert(nullptr != targetTex, FK_FAIL);
        fbo->attach(targetTex, true);
    }

    FK_GL_CHECK(program->bind());
    if (vertexObj) {
        vertexObj->bind();
    }
//    program->addValue(srcTex);
//    program->addValue(mat);
//    program->addValue(vertexObj);
//    program->addValue(pointColor);
//    program->addValue(pointSizeOfPixel);
//    program->addValue(pointShape);
    FK_GL_CHECK(glDrawArrays(_getRenderMode(mode), 0, countVertex));

    if (vertexObj) {
        vertexObj->unbind();
    }
    FK_GL_CHECK(program->clear());
    program->unbind();
    if (_enableSwapBuffers) {
        FK_GL_CHECK(context->swapBuffers());
    } else {
        fbo->unbind();
    }
    if (_enableBlend) {
        glDisable(GL_BLEND);
    }
    glFlush();
    return FK_OK;
}

int FkGraphicRender::_getRenderMode(kRenderMode mode) {
    switch (mode) {
        case kRenderMode::POINTS:
            return GL_POINTS;
        case kRenderMode::TRIANGLE_STRIP:
        default:
            return GL_TRIANGLE_STRIP;
    }
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setContext(
        std::shared_ptr<FkGraphicContext> _context) {
    this->context = _context;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setViewport(int x, int y, int width, int height) {
    size.set(width, height);
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setColor(FkColor _color) {
    this->color = std::make_shared<FkColor>(_color);
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setSrcTexture(
        int index, std::shared_ptr<FkGraphicTexture> tex) {
    srcTex = std::make_shared<FkGraphicTexComponent>();
    srcTex->index = index;
    srcTex->tex = tex;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setVertexObj(
        std::shared_ptr<FkVertexObjectComponent> _vertexObj) {
    int32_t offset = 0;
    FkVertexDesc desc;
    this->vertexObj = _vertexObj;
    if (FK_OK ==
        _vertexObj->getValueLoc(FkVertexObjectComponent::kValueLoc::VERTEX, offset, desc)) {
        this->countVertex = desc.countVertex;
    } else {
        FkLogW(FK_DEF_TAG, "Get vertex count error.");
    }
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setMatrix(std::shared_ptr<FkMatrixComponent> _comp) {
    mat = _comp;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender>
FkGraphicRender::setFrameObject(std::shared_ptr<FkGraphicFrameObject> _fbo) {
    this->fbo = _fbo;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender>
FkGraphicRender::setTargetTexture(std::shared_ptr<FkGraphicTexture> _tex) {
    targetTex = _tex;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::enableBlend(bool enable) {
    this->_enableBlend = enable;
    return shared_from_this();
}

std::shared_ptr<FkGraphicRender> FkGraphicRender::setPointConfig(FkColor _color, FkShape::kType shape, int sizeOfPixel) {
    this->pointColor = std::make_shared<FkColorComponent>();
    this->pointColor->color = _color;
    this->pointSizeOfPixel = std::make_shared<FkSizeComponent>();
    this->pointSizeOfPixel->size.set(sizeOfPixel, sizeOfPixel);
    this->pointShape = std::make_shared<FkShapeComponent>();
    this->pointShape->type = shape;
    return shared_from_this();
}
