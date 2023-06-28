/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-04-02 22:37:57
*/

#include "FkRenderCanvasTexCallbackProto.h"

#include <utility>

FK_IMPL_CLASS_TYPE(FkRenderCanvasTexCallbackProto, FkObject)

FkRenderCanvasTexCallbackProto::FkRenderCanvasTexCallbackProto(
        std::function<void(uint32_t, FkSize, int64_t)> callback)
        : FkProtocol(), callback(std::move(callback)) {

}

FkRenderCanvasTexCallbackProto::FkRenderCanvasTexCallbackProto(
        const FkRenderCanvasTexCallbackProto &o)
        : FkProtocol(o), callback(o.callback) {

}

FkRenderCanvasTexCallbackProto::~FkRenderCanvasTexCallbackProto() {
    callback = nullptr;
}