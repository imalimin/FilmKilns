/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:17:35
*/

#include "FkRenderTexQuark.h"
#include "FkNewTexProto.h"
#include "FkRenderProto.h"
#include "FkGLDefinition.h"
#include "FkSizeCompo.h"
#include "FkColorCompo.h"
#include "FkNewBmpTexProto.h"
#include "FkBufCompo.h"
#include "FkRmMaterialProto.h"

FK_IMPL_CLASS_TYPE(FkRenderTexQuark, FkQuark)

FkRenderTexQuark::FkRenderTexQuark() : FkQuark() {

}

FkRenderTexQuark::~FkRenderTexQuark() {
}

void FkRenderTexQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkNewTexProto, FkRenderTexQuark::_onAllocTex);
    FK_PORT_DESC_QUICK_ADD(desc, FkNewBmpTexProto, FkRenderTexQuark::_onAllocTexWithBmp);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderTexQuark::_onRender);
    FK_PORT_DESC_QUICK_ADD(desc, FkRmMaterialProto, FkRenderTexQuark::_onRemoveTex);
}

FkResult FkRenderTexQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkRenderTexQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderTexQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkGraphicAllocator>(1080 * 1920 * 4 * 10);
    return ret;
}

FkResult FkRenderTexQuark::onStop() {
    sMap.clear();
    allocator->release();
    allocator = nullptr;
    return FkQuark::onStop();
}

FkResult FkRenderTexQuark::_onAllocTex(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkNewTexProto, p);
    if (allocator == nullptr) {
        FkLogE(FK_DEF_TAG, "Texture allocator is null.");
        return FK_FAIL;
    }
    auto tex = _findTex(proto->texEntity->getMaterial()->id());
    if (tex == nullptr) {
        FkTexDescription desc(GL_TEXTURE_2D);
        desc.fmt = proto->texEntity->format();
        desc.size = proto->texEntity->size();
        tex = allocator->alloc(desc);
        if (nullptr == tex) {
            FkLogE(FK_DEF_TAG, "Texture allocator return null.");
            return FK_FAIL;
        }
        sMap.insert(std::make_pair(proto->texEntity->getMaterial()->id(), tex));
    }
    auto bufCompo = FK_FIND_COMPO(proto->texEntity, FkBufCompo);
    if (bufCompo) {
        tex->update(tex->desc.fmt,
                    proto->texEntity->size().getWidth(),
                    proto->texEntity->size().getHeight(),
                    bufCompo->buf->data());
    } else if (tex->desc.size != proto->texEntity->size()) {
        tex->update(tex->desc.fmt,
                    proto->texEntity->size().getWidth(),
                    proto->texEntity->size().getHeight());
    }
    auto ret = _drawColor(tex, proto->texEntity);
    if (FK_OK != ret) {
        FkLogD(FK_DEF_TAG, "Skip draw color. No color component.");
    }
    return FK_OK;
}

FkResult FkRenderTexQuark::_onAllocTexWithBmp(std::shared_ptr<FkProtocol> p) {
    return _onAllocTex(p);
}

FkResult FkRenderTexQuark::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    auto material = proto->materials->getMaterial();
    auto tex = _findTex(material->id());
    if (nullptr == tex) {
        return FK_SOURCE_NOT_FOUND;
    }
    proto->materials->addComponent(std::make_shared<FkTexCompo>(tex));
    auto sizeCompo = FK_FIND_COMPO(proto->materials, FkSizeCompo);
    if (sizeCompo == nullptr) {
        sizeCompo = std::make_shared<FkSizeCompo>(tex->desc.size);
        proto->materials->addComponent(sizeCompo);
    }
    sizeCompo->size = tex->desc.size;
    if (!proto->device->getMaterial()->isUseless()) {
        tex = _findTex(proto->device->getMaterial()->id());
        proto->device->addComponent(std::make_shared<FkTexCompo>(tex));
        sizeCompo = FK_FIND_COMPO(proto->device, FkSizeCompo);
        if (sizeCompo == nullptr) {
            sizeCompo = std::make_shared<FkSizeCompo>(tex->desc.size);
            proto->device->addComponent(sizeCompo);
        }
        sizeCompo->size = tex->desc.size;
    }
    return FK_OK;
}

std::shared_ptr<FkGraphicTexture> FkRenderTexQuark::_findTex(FkID id) {
    auto itr = sMap.find(id);
    if (itr != sMap.end()) {
        return itr->second;
    }
    return nullptr;
}

FkResult FkRenderTexQuark::_drawColor(std::shared_ptr<FkGraphicTexture> &tex,
                                      std::shared_ptr<FkTexEntity> &texEntity) {
    auto colorCompo = FK_FIND_COMPO(texEntity, FkColorCompo);
    auto fboCompo = texEntity->fbo();
    if (colorCompo && fboCompo) {
        auto size = tex->desc.size;
        fboCompo->fbo->bind();
        fboCompo->fbo->attach(tex);
        glViewport(0, 0, size.getWidth(), size.getHeight());
        glClearColor(colorCompo->color.fRed(), colorCompo->color.fGreen(),
                     colorCompo->color.fBlue(), colorCompo->color.fAlpha());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fboCompo->fbo->detach(tex->desc.target);
        fboCompo->fbo->unbind();
        return FK_OK;
    }
    return FK_FAIL;
}

FkResult FkRenderTexQuark::_onRemoveTex(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRmMaterialProto, p);
    if (proto->material == nullptr) {
        return FK_NPE;
    }
    auto itr = sMap.find(proto->material->id());
    if (itr != sMap.end()) {
        sMap.erase(itr);
    }
    return FK_OK;
}
