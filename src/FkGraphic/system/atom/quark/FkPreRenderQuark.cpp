/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkPreRenderQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicCtxComponent.h"
#include "FkGraphicFBOComponent.h"
#include "FkGraphicProgramComponent.h"
#include "FkGraphicTexComponent.h"
#include "FkSizeComponent.h"
#include "FkGraphicRender.h"
#include "FkGLDefinition.h"

//每个点占多少字节
#define SIZE_OF_VERTEX  4
//一个顶点坐标包含几个点
#define COUNT_PER_VERTEX  2
//所有顶点坐标总共多少字节
#define VERTEX_BYTE_SIZE  COUNT_PER_VERTEX * SIZE_OF_VERTEX * sizeof(float)

FkPreRenderQuark::FkPreRenderQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkPreRenderQuark::~FkPreRenderQuark() {

}

void FkPreRenderQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkPreRenderQuark::_onRenderRequest);
}

FkResult FkPreRenderQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkPreRenderQuark::onDestroy() {
    if (GL_NONE != vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = GL_NONE;
    }
    return FkQuark::onDestroy();
}

FkResult FkPreRenderQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkPreRenderQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkPreRenderQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkRenderRequestPrt, p);
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    vec.clear();
    if (FK_OK != proto->req->canvas->findComponent(vec, FkClassType::type<FkGraphicProgramComponent>())) {
        return FK_SKIP;
    }
    auto program = Fk_POINTER_CAST(FkGraphicProgramComponent, vec[0]);

    vec.clear();
    if (FK_OK != proto->req->findComponent(vec, FkClassType::type<FkGraphicCtxComponent>())) {
        return FK_FAIL;
    }
    auto context = Fk_POINTER_CAST(FkGraphicCtxComponent, vec[0]);

    vec.clear();
    if (FK_OK != proto->req->findComponent(vec, FkClassType::type<FkGraphicFBOComponent>())) {
        return FK_FAIL;
    }
    auto fbo = Fk_POINTER_CAST(FkGraphicFBOComponent, vec[0]);

    vec.clear();
    if (FK_OK != proto->req->canvas->findComponent(vec, FkClassType::type<FkGraphicTexComponent>())) {
        return FK_FAIL;
    }
    auto tex = Fk_POINTER_CAST(FkGraphicTexComponent, vec[0]);

    vec.clear();
    if (FK_OK != proto->req->canvas->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        return FK_FAIL;
    }
    auto size = Fk_POINTER_CAST(FkSizeComponent, vec[0]);

    vec.clear();
    context->context->makeCurrent();
    if (GL_NONE == vbo) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BYTE_SIZE * 2, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    }

    float pos[]{
            -1.0f, -1.0f,//LEFT,BOTTOM
            1.0f, -1.0f,//RIGHT,BOTTOM
            -1.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    float coordinate[]{
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    auto ret = FkGraphicRender::with(program->program)
            ->setContext(context->context)
            ->setViewport(0, 0, size->size.getWidth(), size->size.getHeight())
            ->setVertexBuffer(vbo)
            ->setPosition(SIZE_OF_VERTEX, COUNT_PER_VERTEX, 0, pos)
            ->setCoordinate(SIZE_OF_VERTEX, COUNT_PER_VERTEX, VERTEX_BYTE_SIZE, coordinate)
            ->setFrameObject(fbo->fbo)
            ->setTargetTexture(tex->tex)
            ->render();
    return ret;
}