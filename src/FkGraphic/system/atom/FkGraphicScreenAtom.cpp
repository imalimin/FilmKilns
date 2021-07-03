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

//每个点占多少字节
#define SIZE_OF_VERTEX  4
//一个顶点坐标包含几个点
#define COUNT_PER_VERTEX  2
//所有顶点坐标总共多少字节
#define VERTEX_BYTE_SIZE  COUNT_PER_VERTEX * SIZE_OF_VERTEX * 4

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
    // st
    position = new float[8]{
            -1.0f, -1.0f,//LEFT,BOTTOM
            1.0f, -1.0f,//RIGHT,BOTTOM
            -1.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    // xy
    coordinate = new float[8]{
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    return FkSimpleAtom::onCreate();
}

FkResult FkGraphicScreenAtom::onDestroy() {
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
    if (prt->req->layers.empty()) {
        return FK_FAIL;
    }
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
        prt->req->layers[0]->findComponent(vec, FkClassType::type<FkGraphicTexComponent>())) {
        return FK_FAIL;
    }
    tex = Fk_POINTER_CAST(FkGraphicTexComponent, vec[0]);
    vec.clear();
    std::shared_ptr<FkSizeComponent> size = nullptr;
    if (FK_OK != prt->req->layers[0]->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        return FK_FAIL;
    }
    size = Fk_POINTER_CAST(FkSizeComponent, vec[0]);
    vec.clear();
    std::shared_ptr<FkColorComponent> color = nullptr;
    if (FK_OK != prt->req->layers[0]->findComponent(vec, FkClassType::type<FkColorComponent>())) {
        color = std::make_shared<FkColorComponent>();
        color->color = FkColor::white();
    }
    color = Fk_POINTER_CAST(FkColorComponent, vec[0]);
    vec.clear();

    auto texValue = std::make_shared<FkTexValue>();
    texValue->index = 0;
    texValue->tex = tex->tex;
    auto coorValue = std::make_shared<FkCoordinateValue>();
    coorValue->countVertex = SIZE_OF_VERTEX;
    coorValue->countPerVertex = COUNT_PER_VERTEX;
    coorValue->offset = 0;
    auto posValue = std::make_shared<FkPositionValue>();
    coorValue->countVertex = SIZE_OF_VERTEX;
    posValue->countPerVertex = COUNT_PER_VERTEX;
    posValue->offset = SIZE_OF_VERTEX;

    context->context->makeCurrent();
    glViewport(0, 0, size->size.getWidth(), size->size.getHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (GL_NONE == vbo) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BYTE_SIZE * 2, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, VERTEX_BYTE_SIZE, position);
    glBufferSubData(GL_ARRAY_BUFFER, VERTEX_BYTE_SIZE, VERTEX_BYTE_SIZE, coordinate);

    program->bind();
    program->addValue(texValue);
    program->addValue(coorValue);
    program->addValue(posValue);

    FK_GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4))

    program->unbind();
    context->context->swapBuffers();
    glFlush();

    return FK_OK;
}
