/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-11 11:07:20
*/

#include "FkRenderContext.h"
#include "FkGLDefinition.h"

FK_IMPL_CLASS_TYPE(FkRenderContext, FkQuarkContext)

FkRenderContext::FkRenderContext() : FkQuarkContext(),
                                     glVersion(FK_GL_VER_2),
                                     maxTextureSize(0),
                                     maxCountOfFragmentTexture(0),
                                     maxCountOfVertexTexture(0) {

}

FkRenderContext::~FkRenderContext() {

}

void FkRenderContext::setGlVersion(int32_t value) {
    glVersion = value;
}

void FkRenderContext::setMaxTextureSize(int32_t value) {
    maxTextureSize = value;
}

void FkRenderContext::setMaxCountOfFragmentTexture(int32_t value) {
    maxCountOfFragmentTexture = value;
}

void FkRenderContext::setMaxCountOfVertexTexture(int32_t value) {
    maxCountOfVertexTexture = value;
}

int32_t FkRenderContext::getGlVersion() {
    return glVersion;
}

int32_t FkRenderContext::getMaxTextureSize() {
    return maxTextureSize;
}

int32_t FkRenderContext::getMaxCountOfFragmentTexture() {
    return maxCountOfFragmentTexture;
}

int32_t FkRenderContext::getMaxCountOfVertexTexture() {
    return maxCountOfVertexTexture;
}