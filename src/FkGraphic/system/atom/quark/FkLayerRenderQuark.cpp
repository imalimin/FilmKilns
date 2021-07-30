/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLayerRenderQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicCtxComponent.h"
#include "FkGraphicTexComponent.h"
#include "FkGraphicFBOComponent.h"
#include "FkGraphicProgramComponent.h"
#include "FkSizeComponent.h"
#include "FkColorComponent.h"
#include "FkGLDefinition.h"
#include "FkGraphicRender.h"
#include "FkMatrixComponent.h"

//每个点占多少字节
#define SIZE_OF_VERTEX  4
//一个顶点坐标包含几个点
#define COUNT_PER_VERTEX  2
//所有顶点坐标总共多少字节
#define VERTEX_BYTE_SIZE  COUNT_PER_VERTEX * SIZE_OF_VERTEX * sizeof(float)

FkLayerRenderQuark::FkLayerRenderQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkLayerRenderQuark::~FkLayerRenderQuark() {

}

void FkLayerRenderQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkLayerRenderQuark::_onRenderRequest);
}

FkResult FkLayerRenderQuark::onCreate() {
    allocator = std::make_shared<FkGraphicProgramAllocator>();
    return FkQuark::onCreate();
}

FkResult FkLayerRenderQuark::onDestroy() {
    if (GL_NONE != vbo) {
        glDeleteBuffers(1, &vbo);
        vbo = GL_NONE;
    }
    return FkQuark::onDestroy();
}

FkResult FkLayerRenderQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkLayerRenderQuark::onStop() {
    return FkQuark::onStop();
}

FkSize FkLayerRenderQuark::_getCanvasSize(std::shared_ptr<FkGraphicLayer> canvas) {
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    if (FK_OK == canvas->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        return Fk_POINTER_CAST(FkSizeComponent, vec[0])->size;
    }
    return FkSize(1, 1);
}

std::shared_ptr<FkGraphicTexture> FkLayerRenderQuark::_getCanvasTexture(
        std::shared_ptr<FkGraphicLayer> canvas) {
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    if (FK_OK == canvas->findComponent(vec, FkClassType::type<FkGraphicTexComponent>())) {
        return Fk_POINTER_CAST(FkGraphicTexComponent, vec[0])->tex;
    }
    return nullptr;
}

FkResult FkLayerRenderQuark::_drawLayer(std::shared_ptr<FkGraphicLayer> layer,
                                         std::shared_ptr<FkGraphicRender> render) {
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    std::shared_ptr<FkGraphicTexComponent> tex = nullptr;
    std::shared_ptr<FkSizeComponent> size = nullptr;
    std::shared_ptr<FkMatrixComponent> mat = nullptr;
    if (FK_OK != layer->findComponent(vec, FkClassType::type<FkGraphicTexComponent>())) {
        return FK_FAIL;
    }
    tex = Fk_POINTER_CAST(FkGraphicTexComponent, vec[0]);
    vec.clear();
    if (FK_OK != layer->findComponent(vec, FkClassType::type<FkSizeComponent>())) {
        return FK_FAIL;
    }
    size = Fk_POINTER_CAST(FkSizeComponent, vec[0]);
    vec.clear();
    if (FK_OK != layer->findComponent(vec, FkClassType::type<FkMatrixComponent>())) {
        return FK_FAIL;
    }
    mat = Fk_POINTER_CAST(FkMatrixComponent, vec[0]);
    vec.clear();

    float pos[]{
            -size->size.getWidth() / 2.0f, -size->size.getHeight() / 2.0f,//LEFT,BOTTOM
            size->size.getWidth() / 2.0f, -size->size.getHeight() / 2.0f,//RIGHT,BOTTOM
            -size->size.getWidth() / 2.0f, size->size.getHeight() / 2.0f,//LEFT,TOP
            size->size.getWidth() / 2.0f, size->size.getHeight() / 2.0f//RIGHT,TOP
    };
    float coordinate[]{
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    render->setSrcTexture(0, tex->tex)
            ->setMatrix(mat)
            ->setPosition(SIZE_OF_VERTEX, COUNT_PER_VERTEX, 0, pos)
            ->setCoordinate(SIZE_OF_VERTEX, COUNT_PER_VERTEX, VERTEX_BYTE_SIZE, coordinate)
            ->render();
    return FK_OK;
}

FkResult FkLayerRenderQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
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
    std::shared_ptr<FkGraphicProgramComponent> program = nullptr;
    if (FK_OK != prt->req->findComponent(vec, FkClassType::type<FkGraphicProgramComponent>())) {
        return FK_FAIL;
    }
    program = Fk_POINTER_CAST(FkGraphicProgramComponent, vec[0]);

    context->context->makeCurrent();
    if (GL_NONE == vbo) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, VERTEX_BYTE_SIZE * 2, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    }

    auto canvasSize = _getCanvasSize(canvas);
    auto render = FkGraphicRender::with(program->program)
            ->enableSwapBuffers(false)
            ->enableBlend(true)
            ->setContext(context->context)
            ->setViewport(0, 0, canvasSize.getWidth(), canvasSize.getHeight())
            ->setColor(FkColor::white())
            ->setFrameObject(fbo->fbo)
            ->setTargetTexture(_getCanvasTexture(canvas))
            ->setVertexBuffer(vbo);

    for (auto &layer : prt->req->layers) {
        if (Fk_CANVAS_ID == layer->id) {
            continue;
        }
        auto colorComp = layer->findComponent<FkColorComponent>();
        if (nullptr != colorComp) {
            auto texComp = layer->findComponent<FkGraphicTexComponent>();
            if (nullptr != texComp) {
                _drawColor(fbo->fbo, texComp->tex, colorComp->color);
            }
        }
        if (FK_OK != _drawLayer(layer, render)) {
            continue;
        }
    }

    FkProgramDescription desc(FkProgramDescription::kType::POINT);
    auto pointProgramComp = std::make_shared<FkGraphicProgramComponent>();
    pointProgramComp->program = allocator->alloc(desc);

    float pos0[]{0.5f, 0.5f};
    FkGraphicRender::with(pointProgramComp->program)
            ->enableSwapBuffers(false)
            ->setContext(context->context)
            ->setViewport(0, 0, canvasSize.getWidth(), canvasSize.getHeight())
            ->setVertexBuffer(vbo)
            ->setFrameObject(fbo->fbo)
            ->setTargetTexture(_getCanvasTexture(canvas))
            ->setPosition(1, 2, 0, pos0)
            ->render(FkGraphicRender::kRenderMode::POINTS);
    return FK_OK;
}

FkResult FkLayerRenderQuark::_drawColor(std::shared_ptr<FkGraphicFrameObject> fbo,
                                         std::shared_ptr<FkGraphicTexture> tex,
                                         FkColor &color) {
    fbo->attach(tex);
    fbo->bind();
    glClearColor(color.fRed(),
                 color.fGreen(),
                 color.fBlue(),
                 color.fAlpha());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fbo->unbind();
    return FK_OK;
}