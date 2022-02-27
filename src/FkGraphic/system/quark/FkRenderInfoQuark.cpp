/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 23:17:54
*/

#include "FkRenderInfoQuark.h"
#include "FkNewTexProto.h"
#include "FkRenderProto.h"
#include "FkSizeCompo.h"

FkRenderInfoQuark::FkRenderInfoQuark() : FkQuark() {
    FK_MARK_SUPER
}

FkRenderInfoQuark::~FkRenderInfoQuark() {

}

void FkRenderInfoQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkNewTexProto, FkRenderInfoQuark::_onAllocTex);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderInfoQuark::_onRender);
}

FkResult FkRenderInfoQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkRenderInfoQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderInfoQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return FkQuark::onStart();
}

FkResult FkRenderInfoQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkRenderInfoQuark::_onAllocTex(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkNewTexProto, p);
    auto id = proto->texEntity->getMaterial()->id();
    sMap[id] = proto->texEntity->size();
    return FK_OK;
}

FkResult FkRenderInfoQuark::_onRender(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkTexDeviceEntity)) {
        return FK_SKIP;
    }
    auto itr = sMap.find(proto->material->getMaterial()->id());
    if (itr != sMap.end()) {
        proto->material->addComponent(std::make_shared<FkSizeCompo>(itr->second));
    }
    itr = sMap.find(proto->device->getMaterial()->id());
    if (itr != sMap.end()) {
        proto->device->addComponent(std::make_shared<FkSizeCompo>(itr->second));
    }
    return FK_OK;
}
