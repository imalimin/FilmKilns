/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-04-02 22:37:57
*/

#ifndef FKIMAGE_FKRENDERCANVASTEXCALLBACKPROTO_H
#define FKIMAGE_FKRENDERCANVASTEXCALLBACKPROTO_H

#include "FkProtocol.h"
#include "FkSize.h"

FK_SUPER_CLASS(FkRenderCanvasTexCallbackProto, FkProtocol) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderCanvasTexCallbackProto)

public:
    FkRenderCanvasTexCallbackProto(std::function<void(uint32_t, FkSize, int64_t)> callback);

    FkRenderCanvasTexCallbackProto(const FkRenderCanvasTexCallbackProto &o);

    virtual ~FkRenderCanvasTexCallbackProto();

public:
    std::function<void(uint32_t, FkSize, int64_t)> callback = nullptr;
};

#endif //FKIMAGE_FKRENDERCANVASTEXCALLBACKPROTO_H