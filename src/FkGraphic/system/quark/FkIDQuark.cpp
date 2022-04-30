/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-21 23:50:21
*/

#include "FkIDQuark.h"
#include "FkGenIDProto.h"
#include "FkFrameworkDefine.h"

FK_IMPL_CLASS_TYPE(FkIDQuark, FkQuark)

FkIDQuark::FkIDQuark() : FkQuark() {

}

FkIDQuark::~FkIDQuark() {

}

void FkIDQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkGenIDProto, FkIDQuark::_onGenID);
}

FkResult FkIDQuark::onCreate() {
    ++next;
    return FkQuark::onCreate();
}

FkResult FkIDQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkIDQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkIDQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkIDQuark::_onGenID(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkGenIDProto, p);
    std::lock_guard<std::mutex> guard(mtx);
    proto->id = next;
    ++next;
    return FK_OK;
}