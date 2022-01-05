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
    return FkSimpleAtom::onCreate();
}

FkResult FkGraphicScreenAtom::onDestroy() {
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
    auto tex = canvas->findComponent<FkGraphicTexComponent>();
    FkAssert(nullptr != tex, FK_FAIL);
    auto size = canvas->findComponent<FkSizeComponent>();
    FkAssert(nullptr != size, FK_FAIL);
    auto mat = canvas->findComponent<FkMatrixComponent>();
    FkAssert(nullptr != mat, FK_FAIL);
    auto color = canvas->findComponent<FkColorComponent>();
    if (nullptr == color) {
        color = std::make_shared<FkColorComponent>();
        color->color = FkColor::black();
    }
    auto verObj = canvas->findComponent<FkVertexObjectComponent>();
    FkAssert(nullptr != verObj, FK_NPE);

    auto ret = FkGraphicRender::with(program)
            ->enableSwapBuffers(true)
            ->setContext(context->context)
            ->setViewport(0, 0, context->context->getWidth(), context->context->getHeight())
            ->setColor(color->color)
            ->setVertexObj(verObj)
            ->setSrcTexture(0, tex->tex)
            ->setMatrix(mat)
            ->render();
    return ret;
}
