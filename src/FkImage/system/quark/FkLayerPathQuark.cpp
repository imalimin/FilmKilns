/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-5-6 13:30:28
*/

#include "FkLayerPathQuark.h"
#include "FkDrawPathProto.h"

FK_IMPL_CLASS_TYPE(FkLayerPathQuark, FkQuark)

FkLayerPathQuark::FkLayerPathQuark() : FkQuark() {

}

FkLayerPathQuark::~FkLayerPathQuark() {

}

void FkLayerPathQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkDrawPathProto, FkLayerPathQuark::_onDrawPath);
}

FkResult FkLayerPathQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkLayerPathQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkLayerPathQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkLayerPathQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkLayerPathQuark::_onDrawPath(std::shared_ptr<FkProtocol> &p) {
    return FK_OK;
}