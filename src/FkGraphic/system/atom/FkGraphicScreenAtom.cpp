/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicScreenAtom.h"
#include "FkGraphicLayerPrt.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicCtxComponent.h"
#include "FkGraphicTexComponent.h"
#include "FkGraphicFBOComponent.h"
#include "FkGraphicProgramComponent.h"
#include "FkSizeComponent.h"
#include "FkColorComponent.h"
#include "FkGLDefinition.h"
#include "FkGraphicMatProgram.h"
#include "FkTexValue.h"
#include "FkCoordinateValue.h"
#include "FkPositionValue.h"
#include "FkGraphicRender.h"
#include "FkMatrixComponent.h"
#include "ext.hpp"

//每个点占多少字节
#define SIZE_OF_VERTEX  4
//一个顶点坐标包含几个点
#define COUNT_PER_VERTEX  2
//所有顶点坐标总共多少字节
#define VERTEX_BYTE_SIZE  COUNT_PER_VERTEX * SIZE_OF_VERTEX * sizeof(float)

FkGraphicScreenAtom::FkGraphicScreenAtom() : FkSimpleAtom() {
    FK_MARK_SUPER

}

FkGraphicScreenAtom::~FkGraphicScreenAtom() {

}

void FkGraphicScreenAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicScreenAtom::_onRenderRequest);
}

void FkGraphicScreenAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
}

FkResult FkGraphicScreenAtom::onCreate() {
    // xyz
    position = new float[8]{
            -1.0f, -1.0f,//LEFT,BOTTOM
            1.0f, -1.0f,//RIGHT,BOTTOM
            -1.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    // uv
    coordinate = new float[8]{
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    return FkSimpleAtom::onCreate();
}

FkResult FkGraphicScreenAtom::onDestroy() {
    if (GL_NONE != vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = GL_NONE;
    }
    if (position) {
        delete[] position;
        position = nullptr;
    }
    if (coordinate) {
        delete[] coordinate;
        coordinate = nullptr;
    }
    return FkSimpleAtom::onDestroy();
}

FkResult FkGraphicScreenAtom::onStart() {
    return FkSimpleAtom::onStart();
}

FkResult FkGraphicScreenAtom::onStop() {
    return FkSimpleAtom::onStop();
}

FkResult FkGraphicScreenAtom::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkRenderRequestPrt, p);
    auto canvas = prt->req->getCanvas();
    FkAssert(nullptr != canvas, FK_FAIL);
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    std::shared_ptr<FkGraphicCtxComponent> context = nullptr;
    if (FK_OK != prt->req->findComponent(vec, FkClassType::type<FkGraphicCtxComponent>())) {
        return FK_FAIL;
    }
    context = Fk_POINTER_CAST(FkGraphicCtxComponent, vec[0]);
    vec.clear();
    std::shared_ptr<FkGraphicFBOComponent> fbo = nullptr;
    if (FK_OK != prt->req->findComponent(vec, FkClassType::type<FkGraphicFBOComponent>())) {
        return FK_FAIL;
    }
    fbo = Fk_POINTER_CAST(FkGraphicFBOComponent, vec[0]);
    vec.clear();
    std::shared_ptr<FkGraphicProgramComponent> programCom = nullptr;
    if (FK_OK != prt->req->findComponent(vec, FkClassType::type<FkGraphicProgramComponent>())) {
        return FK_FAIL;
    }
    programCom = Fk_POINTER_CAST(FkGraphicProgramComponent, vec[0]);
    auto program = programCom->program;
    vec.clear();
    std::shared_ptr<FkGraphicTexComponent> tex = nullptr;
    if (FK_OK !=
        canvas->findComponent(vec, FkClassType::type<FkGraphicTexComponent>())) {
        return FK_FAIL;
    }
    tex = Fk_POINTER_CAST(FkGraphicTexComponent, vec[0]);
    vec.clear();
    std::shared_ptr<FkSizeComponent> size = nullptr;
    if (FK_OK != canvas->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        return FK_FAIL;
    }
    size = Fk_POINTER_CAST(FkSizeComponent, vec[0]);
    vec.clear();
    std::shared_ptr<FkMatrixComponent> mat = nullptr;
    if (FK_OK != canvas->findComponent(vec, FkClassType::type<FkMatrixComponent>())) {
        return FK_FAIL;
    }
    mat = Fk_POINTER_CAST(FkMatrixComponent, vec[0]);
    vec.clear();

    std::shared_ptr<FkColorComponent> color = nullptr;
    if (FK_OK != canvas->findComponent(vec, FkClassType::type<FkColorComponent>())) {
        color = std::make_shared<FkColorComponent>();
        color->color = FkColor::black();
    } else {
        color = Fk_POINTER_CAST(FkColorComponent, vec[0]);
    }
    vec.clear();

    if (GL_NONE == vbo) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BYTE_SIZE * 2, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    }

    float pos[]{
            -size->size.getWidth() / 2.0f, -size->size.getHeight() / 2.0f,//LEFT,BOTTOM
            size->size.getWidth() / 2.0f, -size->size.getHeight() / 2.0f,//RIGHT,BOTTOM
            -size->size.getWidth() / 2.0f, size->size.getHeight() / 2.0f,//LEFT,TOP
            size->size.getWidth() / 2.0f, size->size.getHeight() / 2.0f//RIGHT,TOP
    };
    auto ret = FkGraphicRender::with(program)
            ->enableSwapBuffers(true)
            ->setContext(context->context)
            ->setViewport(0, 0, context->context->getWidth(), context->context->getHeight())
            ->setColor(color->color)
            ->setVertexBuffer(vbo)
            ->setSrcTexture(0, tex->tex)
            ->setMatrix(mat->value)
            ->setPosition(SIZE_OF_VERTEX, COUNT_PER_VERTEX, 0, pos)
            ->setCoordinate(SIZE_OF_VERTEX, COUNT_PER_VERTEX, VERTEX_BYTE_SIZE, coordinate)
            ->render();
    return ret;
}
