/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkPreRenderQuark.h"
#include "FkRenderRequestPrt.h"

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
    return FkQuark::onDestroy();
}

FkResult FkPreRenderQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkPreRenderQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkPreRenderQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {

}