/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderRequest.h"

FkRenderRequest::FkRenderRequest() : FkGraphicEntity() {
    FK_MARK_SUPER
}

FkRenderRequest::FkRenderRequest(const FkRenderRequest &o) : FkGraphicEntity(o) {
    FK_MARK_SUPER
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
