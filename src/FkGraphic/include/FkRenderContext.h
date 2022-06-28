/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-11 11:07:20
*/

#ifndef FK_GRAPHIC_FKRENDERCONTEXT_H
#define FK_GRAPHIC_FKRENDERCONTEXT_H

#include "FkQuarkContext.h"
#include "FkSize.h"

FK_SUPER_CLASS(FkRenderContext, FkQuarkContext) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderContext)

public:
    FkRenderContext();

    FkRenderContext(const FkRenderContext &o) = delete;

    virtual ~FkRenderContext();

    void setGlVersion(int32_t value);

    void setMaxTextureSize(int32_t value);

    void setMaxCountOfFragmentTexture(int32_t value);

    void setMaxCountOfVertexTexture(int32_t value);

    void setMaxViewportSize(int32_t width, int32_t height);

    int32_t getGlVersion();

    int32_t getMaxTextureSize();

    int32_t getMaxCountOfFragmentTexture();

    int32_t getMaxCountOfVertexTexture();

    FkSize getMaxViewportSize();

private:
    int32_t glVersion;
    int32_t maxTextureSize;
    int32_t maxCountOfFragmentTexture;
    int32_t maxCountOfVertexTexture;
    FkSize maxViewportSize;
};

#endif //FK_GRAPHIC_FKRENDERCONTEXT_H