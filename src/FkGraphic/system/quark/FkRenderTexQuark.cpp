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

FkRenderTexQuark::FkRenderTexQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkRenderTexQuark::~FkRenderTexQuark() {

}

void FkRenderTexQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkNewTexProto, FkRenderTexQuark::_onNewTex);
}

FkResult FkRenderTexQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkRenderTexQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderTexQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkRenderTexQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkRenderTexQuark::_onNewTex(std::shared_ptr<FkProtocol> p) {
    return FK_OK;
}