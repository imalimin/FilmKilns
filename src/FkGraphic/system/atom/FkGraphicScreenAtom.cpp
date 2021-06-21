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
    std::shared_ptr<FkGraphicMatProgram> program = std::static_pointer_cast<FkGraphicMatProgram>(
            programCom->program);
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

    context->context->makeCurrent();
    glViewport(0, 0, size->size.getWidth(), size->size.getHeight());
    glClearColor(color->color.fRed(), color->color.fGreen(), color->color.fBlue(), 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program->bind();
    program->bindTexture(GL_TEXTURE_2D, 0, tex->tex->tex);

    if (GL_NONE == vbo) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BYTE_SIZE * 2, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, VERTEX_BYTE_SIZE, position);
    glBufferSubData(GL_ARRAY_BUFFER, VERTEX_BYTE_SIZE, VERTEX_BYTE_SIZE, coordinate);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(program->aPositionLocation);
    glEnableVertexAttribArray(program->aTextureCoordinateLocation);
    //xy
    glVertexAttribPointer(program->aPositionLocation, COUNT_PER_VERTEX, GL_FLOAT, GL_FALSE, 0, 0);
    //st
    glVertexAttribPointer(program->aTextureCoordinateLocation, COUNT_PER_VERTEX, GL_FLOAT, GL_FALSE,
                          0,
                          reinterpret_cast<const void *>(VERTEX_BYTE_SIZE));
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(program->aPositionLocation);
    glDisableVertexAttribArray(program->aTextureCoordinateLocation);

    program->bindTexture(GL_TEXTURE_2D, 0, GL_NONE);
    program->unbind();
    glFlush();
    context->context->swapBuffers();

    return FK_OK;
}
