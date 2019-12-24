/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include <cassert>
#include "../include/BaseMultipleSamplerFilter.h"
#include "../include/NormalDrawer.h"
#include "AlBitmapFactory.h"
#include "log.h"

BaseMultipleSamplerFilter::BaseMultipleSamplerFilter(char **names, char **samplers, int size) {
    assert(nullptr != names && nullptr != samplers);
    this->names = names;
    this->samplers = samplers;
    this->size = size;
}

BaseMultipleSamplerFilter::~BaseMultipleSamplerFilter() {
    if (textures) {
        glDeleteTextures(size, textures);
        delete[]textures;
        textures = nullptr;
    }
    if (textureLocations) {
        delete[]textureLocations;
        textureLocations = nullptr;
    }
}

bool BaseMultipleSamplerFilter::init(int w, int h) {
    if (!Filter::init(w, h))
        return false;
    for (int i = 0; i < size; ++i) {
        LOGI("BaseMultipleSamplerFilter: %s, %s", names[i], samplers[i]);
    }
    drawer = getDrawer();
    textures = new GLuint[size];
    textureLocations = new GLint[size];
    for (int i = 0; i < size; ++i) {
        textures[i] = loadTexture(string(samplers[i]));
        textureLocations[i] = drawer->getUniformLocation(string(names[i]));
    }
    releaseStringArray(names, size);
    releaseStringArray(samplers, size);
    return true;
}

GLuint BaseMultipleSamplerFilter::loadTexture(string path) {
    HwBitmap *bmp = AlBitmapFactory::decodeFile(path);
    if (nullptr == bmp) {
        LOGE("Read image failed");
        return GL_NONE;
    }
    GLuint texture = GL_NONE;
    glGenTextures(1, &texture);
    if (GL_NONE == texture)
        return GL_NONE;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmp->getWidth(), bmp->getHeight(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, bmp->getPixels());
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    delete bmp;
    return texture;
}

void BaseMultipleSamplerFilter::bindResources() {
    Filter::bindResources();
    /**
     * GL_TEXTURE0为保留Sampler，给默认画面使用
     */
    for (int i = 0; i < size; ++i) {
        int offset = i + 1;
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + offset));
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glUniform1i(textureLocations[i], offset);
    }
}

void BaseMultipleSamplerFilter::releaseStringArray(char **array, int size) {
    for (int i = 0; i < size; ++i) {
        free(array[i]);
        free(*array);
        free(array);
    }
}