/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-09-02 00:00:04
*/

#include "FkRenderSourceTextQuark.h"
#include "FkRenderProto.h"
#include "FkTextCompo.h"
#include "FkCharMapCompo.h"

FK_IMPL_CLASS_TYPE(FkRenderSourceTextQuark, FkQuark)

FkRenderSourceTextQuark::FkRenderSourceTextQuark() : FkQuark() {

}

FkRenderSourceTextQuark::~FkRenderSourceTextQuark() {

}

void FkRenderSourceTextQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderSourceTextQuark::_onRender);
}

FkResult FkRenderSourceTextQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    return ret;
}

FkResult FkRenderSourceTextQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderSourceTextQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderSourceTextQuark::onStop() {
    mTexCharMap = nullptr;
    return FkQuark::onStop();
}

FkResult FkRenderSourceTextQuark::_onRender(const std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    std::vector<std::shared_ptr<FkComponent>> textCompoVec;
    if (proto->materials->findComponents(textCompoVec, FkTextCompo_Class::type) == FK_OK) {
        if (mTexCharMap == nullptr) {
            mTexCharMap = std::make_shared<FkTextureCharMap>(16, FkColor::white());
        }
        if (mTexCharMap) {
            proto->materials->addComponent(std::make_shared<FkCharMapCompo>(mTexCharMap));
        }
    }
    return FK_OK;
}