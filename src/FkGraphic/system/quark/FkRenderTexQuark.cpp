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
#include "FkGLDefinition.h"

FkRenderTexQuark::FkRenderTexQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkRenderTexQuark::~FkRenderTexQuark() {

}

void FkRenderTexQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkNewTexProto, FkRenderTexQuark::_onAllocTex);
}

FkResult FkRenderTexQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkRenderTexQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderTexQuark::onStart() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkGraphicAllocator>();
    fboAllocator  = std::make_shared<FkGraphicFBOAllocator>();
    return FkQuark::onStart();
}

FkResult FkRenderTexQuark::onStop() {
    sMap.clear();
    allocator->release();
    return FkQuark::onStop();
}

FkResult FkRenderTexQuark::_onAllocTex(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkNewTexProto, p);
    if (allocator == nullptr) {
        FkLogE(FK_DEF_TAG, "Texture allocator is null.");
        return FK_FAIL;
    }
    FkTexDescription desc(GL_TEXTURE_2D);
    desc.fmt = proto->texEntity->format();
    desc.size = proto->texEntity->size();
    auto tex = allocator->alloc(desc);
    if (nullptr == tex) {
        FkLogE(FK_DEF_TAG, "Texture allocator return null.");
        return FK_FAIL;
    }
    sMap.insert(std::make_pair(proto->texEntity->getMaterial()->id(), tex));
    return FK_OK;
}