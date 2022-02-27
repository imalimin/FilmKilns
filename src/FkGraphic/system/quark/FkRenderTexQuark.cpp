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

FkRenderTexQuark::FkRenderTexQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkRenderTexQuark::~FkRenderTexQuark() {
}

void FkRenderTexQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkNewTexProto, FkRenderTexQuark::_onAllocTex);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderTexQuark::_onRender);
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
    allocator = std::make_shared<FkGraphicAllocator>();
    return FkQuark::onStart();
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
    if (tex->desc.size != proto->texEntity->size()) {
        tex->update(tex->desc.fmt,
                    proto->texEntity->size().getWidth(),
                    proto->texEntity->size().getHeight());
    }
    auto colorCompo = proto->texEntity->findComponent<FkColorCompo>();
    auto fboCompo = proto->texEntity->fbo();
    if (colorCompo && fboCompo) {
        auto size = tex->desc.size;
        glFinish();
        fboCompo->fbo->attach(tex, true);
        glViewport(0, 0, size.getWidth(), size.getHeight());
        glClearColor(colorCompo->color.fRed(), colorCompo->color.fGreen(),
                     colorCompo->color.fBlue(), colorCompo->color.fAlpha());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fboCompo->fbo->detach(tex->desc.target);
    }
    return FK_OK;
}

FkResult FkRenderTexQuark::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    auto material = proto->material->getMaterial();
    auto tex = _findTex(material->id());
    if (nullptr == tex) {
        return FK_SOURCE_NOT_FOUND;
    }
    proto->material->addComponent(std::make_shared<FkTexCompo>(tex));
    auto sizeCompo = proto->material->findComponent<FkSizeCompo>();
    if (sizeCompo == nullptr) {
        sizeCompo = std::make_shared<FkSizeCompo>(tex->desc.size);
        proto->material->addComponent(sizeCompo);
    }
    sizeCompo->size = tex->desc.size;
    if (!proto->device->getMaterial()->isUseless()) {
        tex = _findTex(proto->device->getMaterial()->id());
        proto->device->addComponent(std::make_shared<FkTexCompo>(tex));
        sizeCompo = proto->device->findComponent<FkSizeCompo>();
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
