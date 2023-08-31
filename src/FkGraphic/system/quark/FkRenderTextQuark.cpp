/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-08-31 13:32:27
*/

#include "FkRenderTextQuark.h"
#include "FkRenderProto.h"

FK_IMPL_CLASS_TYPE(FkRenderTextQuark, FkQuark)

FkRenderTextQuark::FkRenderTextQuark() : FkQuark() {

}

FkRenderTextQuark::~FkRenderTextQuark() {

}
void FkRenderTextQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderTextQuark::_onRender);
}

FkResult FkRenderTextQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    return ret;
}

FkResult FkRenderTextQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderTextQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderTextQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkRenderTextQuark::_onRender(const std::shared_ptr<FkProtocol> &p) {
    return FK_OK;
}