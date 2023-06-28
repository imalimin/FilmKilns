/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-07-08 21:39:19
*/

#include "FkRenderDeviceRequest.h"

FK_IMPL_CLASS_TYPE(FkRenderDeviceRequest, FkObject)

FkRenderDeviceRequest::FkRenderDeviceRequest() : FkObject() {

}

FkRenderDeviceRequest::~FkRenderDeviceRequest() {
    vec.clear();
}

size_t FkRenderDeviceRequest::size() {
    return vec.size();
}

FkResult FkRenderDeviceRequest::add(std::shared_ptr<FkMaterialEntity> &materials,
                                    std::shared_ptr<FkDeviceEntity> &device) {
    if (materials == nullptr || device == nullptr) {
        return FK_NPE;
    }
    vec.emplace_back(std::make_pair(materials, device));
    return FK_OK;
}

std::pair<std::shared_ptr<FkMaterialEntity>, std::shared_ptr<FkDeviceEntity>> &
FkRenderDeviceRequest::get(int32_t index) {
    return vec[index];
}
