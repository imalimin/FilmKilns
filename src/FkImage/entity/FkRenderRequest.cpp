/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderRequest.h"
#include "FkString.h"

FK_IMPL_CLASS_TYPE(FkRenderRequest, FkGraphicEntity)

FkRenderRequest::FkRenderRequest() : FkGraphicEntity() {

}

FkRenderRequest::FkRenderRequest(const FkRenderRequest &o) : FkGraphicEntity(o) {

    for (const auto & layer : o.layers) {
        layers.emplace_back(layer);
    }
}

FkRenderRequest::~FkRenderRequest() {
    layers.clear();
}

std::shared_ptr<FkGraphicLayer> FkRenderRequest::getCanvas() {
    auto itr = std::find_if(layers.begin(), layers.end(),
                            [](std::shared_ptr<FkGraphicLayer> item) -> bool {
                                return Fk_CANVAS_ID == item->id;
                            });
    return *itr;
}

std::shared_ptr<FkGraphicLayer> FkRenderRequest::findLayerById(FkID layerId) {
    for (auto &it: layers) {
        if (it->id == layerId) {
            return it;
        }
    }
    return nullptr;
}

std::string FkRenderRequest::toString() {
    FkString str;
    for (auto &it: layers) {
        str.append(it->id);
        str.append(',');
    }
    return str.c_str();
}
