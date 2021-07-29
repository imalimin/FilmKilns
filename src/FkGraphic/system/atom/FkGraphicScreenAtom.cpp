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
#include "FkCoordinateComponent.h"
#include "FkPositionComponent.h"
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
    auto proto = Fk_POINTER_CAST(FkRenderRequestPrt, p);
    auto canvas = proto->req->getCanvas();
    FkAssert(nullptr != canvas, FK_FAIL);
    auto context = proto->req->findComponent<FkGraphicCtxComponent>();
    FkAssert(nullptr != context, FK_FAIL);
    auto fbo = proto->req->findComponent<FkGraphicFBOComponent>();
    FkAssert(nullptr != fbo, FK_FAIL);
    auto programCom = proto->req->findComponent<FkGraphicProgramComponent>();
    FkAssert(nullptr != programCom, FK_FAIL);
    auto program = programCom->program;
    auto tex = proto->req->findComponent<FkGraphicTexComponent>();
    FkAssert(nullptr != tex, FK_FAIL);
    auto size = proto->req->findComponent<FkSizeComponent>();
    FkAssert(nullptr != size, FK_FAIL);
    auto mat = proto->req->findComponent<FkMatrixComponent>();
    FkAssert(nullptr != mat, FK_FAIL);
    auto color = proto->req->findComponent<FkColorComponent>();
    if (nullptr == color) {
        color = std::make_shared<FkColorComponent>();
        color->color = FkColor::black();
    }

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
            ->setMatrix(mat)
            ->setPosition(SIZE_OF_VERTEX, COUNT_PER_VERTEX, 0, pos)
            ->setCoordinate(SIZE_OF_VERTEX, COUNT_PER_VERTEX, VERTEX_BYTE_SIZE, coordinate)
            ->render();
    return ret;
}
