/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-17 22:30:34
*/

#ifndef FK_GRAPHIC_FKRENDERENGINESETTINGS_H
#define FK_GRAPHIC_FKRENDERENGINESETTINGS_H

#include "FkEngineSettings.h"
#include "FkSize.h"
#include "FkGLDefinition.h"
#include <vector>

FK_SUPER_CLASS(FkRenderEngineSettings, FkEngineSettings) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderEngineSettings)

public:
    friend class FkGLEnvAtom;

public:
    FkRenderEngineSettings();

    FkRenderEngineSettings(const FkRenderEngineSettings &o);

    virtual ~FkRenderEngineSettings();

    virtual void setSupportBlockSizeArray(std::vector<int> &vec);

    virtual std::vector<int> getSupportBlockSizeArray();

    void setGlVersion(int32_t value);

    int32_t getGlVersion();

    int32_t getMaxTextureSize();

    int32_t getMaxCountOfFragmentTexture();

    int32_t getMaxCountOfVertexTexture();

    FkSize getMaxViewportSize();

private:
    void setMaxTextureSize(int32_t value);

    void setMaxCountOfFragmentTexture(int32_t value);

    void setMaxCountOfVertexTexture(int32_t value);

    void setMaxViewportSize(int32_t width, int32_t height);

private:
    std::vector<int> supportBlockArray;
    int32_t glVersion = FK_GL_VER_3;
    int32_t maxTextureSize = 0;
    int32_t maxCountOfFragmentTexture = 0;
    int32_t maxCountOfVertexTexture = 0;
    FkSize maxViewportSize = 0;
};

#endif //FK_GRAPHIC_FKRENDERENGINESETTINGS_H