/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicTexQuark.h"
#include "FkGraphicNewTexPtl.h"
#include "FkTexComponent.h"
#include "FkGraphicTexComponent.h"
#include "FkRenderRequestPrt.h"
#include "FkGraphicUpdateTexPrt.h"
#include "FkGLDefinition.h"
#include "FkGraphicTexDelPtl.h"
#include "FkUpdateTexWithBmpPrt.h"

#define DUMP_TEX 0

FkGraphicTexQuark::FkGraphicTexQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkGraphicTexQuark::~FkGraphicTexQuark() {

}

void FkGraphicTexQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicNewTexPtl, FkGraphicTexQuark::_onAllocTex);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateTexPrt, FkGraphicTexQuark::_onUpdateTex);
    FK_PORT_DESC_QUICK_ADD(desc, FkUpdateTexWithBmpPrt, FkGraphicTexQuark::_onUpdateTexWithBitmap);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicTexDelPtl, FkGraphicTexQuark::_onDeleteTex);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicTexQuark::_onRenderRequest);
}

FkResult FkGraphicTexQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkGraphicAllocator>();
    fboAllocator  = std::make_shared<FkGraphicFBOAllocator>();
    return ret;
}

FkResult FkGraphicTexQuark::onDestroy() {
    sMap.clear();
    allocator->release();
    return FkQuark::onDestroy();
}

FkResult FkGraphicTexQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicTexQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicTexQuark::_findTexture(std::shared_ptr<FkGraphicLayer> layer) {
    std::vector<std::shared_ptr<FkGraphicComponent>> vec;
    std::shared_ptr<FkTexComponent> tex = nullptr;
    if (FK_OK != layer->findComponent(vec, FkClassType::type<FkTexComponent>())) {
        return FK_FAIL;
    }
    tex = Fk_POINTER_CAST(FkTexComponent, vec[0]);
    auto itr = sMap.find(tex->id);
    if (itr != sMap.end()) {
        auto comp = std::make_shared<FkGraphicTexComponent>();
        comp->tex = itr->second;
        layer->addComponent(comp);
    }
    return FK_OK;
}

FkResult FkGraphicTexQuark::_onAllocTex(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkGraphicTexPtl, p);
    FkTexDescription desc(GL_TEXTURE_2D);
    desc.fmt = prt->fmt;
    auto tex = allocator->alloc(desc);
    if (nullptr == tex) {
        return FK_FAIL;
    }
    sMap.insert(std::make_pair(tex->id, tex));
    prt->id = tex->id;
    return FK_OK;
}

FkResult FkGraphicTexQuark::_onDeleteTex(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkGraphicTexDelPtl, p);
    auto itr = sMap.find(prt->id);
    if (itr != sMap.end()) {
        sMap.erase(itr);
    }
    prt->id = FK_ID_NONE;
    return FK_OK;
}

FkResult FkGraphicTexQuark::_onUpdateTex(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkGraphicUpdateTexPrt, p);
    if (prt->size.getWidth() <= 0 || prt->size.getHeight() <= 0) {
        FkLogE(FK_DEF_TAG, "Invalid texture size(%dx%d).",
               prt->size.getWidth(),
               prt->size.getHeight());
        return FK_FAIL;
    }
    auto itr = sMap.find(prt->id);
    if (sMap.end() == itr) {
        FkLogE(FK_DEF_TAG, "Texture(id=%d) not found.", prt->id);
        return FK_FAIL;
    }
    itr->second->update(itr->second->desc.fmt, prt->size.getWidth(), prt->size.getHeight());
    return FK_OK;
}

FkResult FkGraphicTexQuark::_onUpdateTexWithBitmap(std::shared_ptr<FkProtocol> p) {
    auto prt = Fk_POINTER_CAST(FkUpdateTexWithBmpPrt, p);
    auto itr = sMap.find(prt->id);
    if (sMap.end() == itr) {
        FkLogE(FK_DEF_TAG, "Texture(id=%d) not found.", prt->id);
        return FK_FAIL;
    }
    itr->second->update(itr->second->desc.fmt, prt->bmp->getWidth(), prt->bmp->getHeight(), prt->bmp->getPixels());

#if DUMP_TEX
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, itr->second->tex, 0);
    uint8_t *buffer = new uint8_t[prt->bmp->getWidth() * prt->bmp->getHeight() * 4];
    glReadPixels(0, 0, prt->bmp->getWidth(), prt->bmp->getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    auto ret = FkBitmap::write("/sdcard/000000.bmp", buffer, prt->bmp->getWidth() * prt->bmp->getHeight() * 4,
                               prt->bmp->getWidth(), prt->bmp->getHeight());
    FkAssert( ret == FK_OK,);

    delete[] buffer;
    glDeleteFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
    return FK_OK;
}

FkResult FkGraphicTexQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkRenderRequestPrt, p);
    auto canvas = proto->req->getCanvas();
    FkAssert(nullptr != canvas, FK_FAIL);
    _findTexture(canvas);
    for (auto &layer : proto->req->layers) {
        _findTexture(layer);
    }
    return FK_OK;
}
