/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-17 22:30:34
*/

#include "FkRenderEngineSettings.h"

FK_IMPL_CLASS_TYPE(FkRenderEngineSettings, FkEngineSettings)

FkRenderEngineSettings::FkRenderEngineSettings() : FkEngineSettings() {
    supportBlockArray = {4096, 1024, 512};
}

FkRenderEngineSettings::FkRenderEngineSettings(const FkRenderEngineSettings &o)
        : FkEngineSettings(o),
          supportBlockArray(o.supportBlockArray) {

}

FkRenderEngineSettings::~FkRenderEngineSettings() {

}

void FkRenderEngineSettings::setSupportBlockSizeArray(std::vector<int> &vec) {
    supportBlockArray = vec;
}

std::vector<int> FkRenderEngineSettings::getSupportBlockSizeArray() {
    return supportBlockArray;
}

void FkRenderEngineSettings::setGlVersion(int32_t value) {
    glVersion = value;
}

void FkRenderEngineSettings::setMaxTextureSize(int32_t value) {
    maxTextureSize = value;
}

void FkRenderEngineSettings::setMaxCountOfFragmentTexture(int32_t value) {
    maxCountOfFragmentTexture = value;
}

void FkRenderEngineSettings::setMaxCountOfVertexTexture(int32_t value) {
    maxCountOfVertexTexture = value;
}

void FkRenderEngineSettings::setMaxViewportSize(int32_t width, int32_t height) {
    maxViewportSize.set(width, height);
}

int32_t FkRenderEngineSettings::getGlVersion() {
    return glVersion;
}

int32_t FkRenderEngineSettings::getMaxTextureSize() {
    return maxTextureSize;
}

int32_t FkRenderEngineSettings::getMaxCountOfFragmentTexture() {
    return maxCountOfFragmentTexture;
}

int32_t FkRenderEngineSettings::getMaxCountOfVertexTexture() {
    return maxCountOfVertexTexture;
}

FkSize FkRenderEngineSettings::getMaxViewportSize() {
    return maxViewportSize;
}
