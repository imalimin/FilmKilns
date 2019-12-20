/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwAndroidTexture.h"
#include "HwAHardwareBuffer.h"
#include "HwAndroidGraphicBuffer.h"
#include "HwAndroidUtils.h"
#include "Logcat.h"
#include "../../../include/HwTexture.h"
#include <GLES2/gl2.h>

#define TAG "HwAndroidTexture"

HwAndroidTexture::HwAndroidTexture(uint32_t target) : HwAbsTexture(
        AlTexDescription(target, GL_RGBA)) {
    sdk = HwAndroidUtils::getAndroidApi();
    if (support()) {
        glGenTextures(1, &tex);
        bind();
        glTexParameterf(desc.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(desc.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(desc.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(desc.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        unbind();
    } else {
        texCompat = HwTexture::alloc(desc);
        tex = texCompat->texId();
    }
}

HwAndroidTexture::~HwAndroidTexture() {
    if (GL_NONE != tex && glIsTexture(tex)) {
        glDeleteTextures(1, &tex);
    }
    if (graphicBuffer) {
        delete graphicBuffer;
        graphicBuffer = nullptr;
    }
    if (texCompat) {
        delete texCompat;
        texCompat = nullptr;
    }
}

void HwAndroidTexture::bind() {
    if (texCompat) {
        texCompat->bind();
    } else {
        glBindTexture(desc.target, tex);
    }
}

void HwAndroidTexture::unbind() {
    if (texCompat) {
        texCompat->unbind();
    } else {
        glBindTexture(desc.target, GL_NONE);
    }
}

void HwAndroidTexture::update(HwBuffer *buf, int32_t w, int32_t h, uint32_t fmt) {
    if (GL_RGBA != fmt || w <= 0 || h <= 0) {
        Logcat::e(TAG, "HwAndroidTexture::update Only support RGBA fmt.");
        return;
    }
    HwAbsTexture::update(buf, w, h, fmt);
    /**
     * Support GraphicBuffer.
     * sdk >= 26: AHardwareBuffer.
     * sdk == 24 || sdk == 25: Normal.
     * sdk <= 23: GraphicBuffer.
     */
    if (!texCompat) {
        glBindTexture(desc.target, tex);
        if (sdk >= 26) {
            graphicBuffer = new HwAHardwareBuffer(w, h);
        } else if (sdk <= 23) {
            graphicBuffer = new HwAndroidGraphicBuffer(w, h);
        } else {
            return;
        }
        graphicBuffer->bind();
        glBindTexture(desc.target, GL_NONE);
    } else {
        texCompat->update(buf, w, h);
    }
}

bool HwAndroidTexture::support() {
    return sdk >= 26 || sdk <= 23;
}

bool HwAndroidTexture::read(uint8_t *pixels) {
    if (graphicBuffer) {
        return graphicBuffer->read(pixels);
    }
    return texCompat->read(pixels);
}
