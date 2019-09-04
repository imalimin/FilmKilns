/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwAndroidTexture.h"
#include "HwAHardwareBuffer.h"
#include "HwAndroidGraphicBuffer.h"
#include "HwAndroidUtils.h"
#include "Logcat.h"
#include "../../../include/HwTexture.h"
#include <GLES2/gl2.h>

HwAndroidTexture::HwAndroidTexture(uint32_t target) : HwAbsTexture(target) {
    sdk = HwAndroidUtils::getAndroidApi();
    if (support()) {
        glGenTextures(1, &tex);
        bind();
        glTexParameterf(tar, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(tar, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(tar, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(tar, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        unbind();
    } else {
        texCompat = HwTexture::alloc(tar);
        tex = texCompat->texId();
    }
}

HwAndroidTexture::~HwAndroidTexture() {
    if (GL_NONE != tex && glIsTexture(tex)) {
        glDeleteTextures(1, &tex);
    }
    size.width = 0;
    size.height = 0;
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
        glBindTexture(tar, tex);
    }
}

void HwAndroidTexture::unbind() {
    if (texCompat) {
        texCompat->unbind();
    } else {
        glBindTexture(tar, GL_NONE);
    }
}

void HwAndroidTexture::update(HwBuffer *buf, int32_t w, int32_t h, uint32_t fmt) {
    if (GL_RGBA != fmt) {
        Logcat::e("hwvc", "HwAndroidTexture::update Only support RGBA fmt.");
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
        glBindTexture(tar, tex);
        if (sdk >= 26) {
            graphicBuffer = new HwAHardwareBuffer(w, h);
        } else if (sdk <= 23) {
            graphicBuffer = new HwAndroidGraphicBuffer(w, h);
        } else {
            return;
        }
        static_cast<HwAbsGraphicBuffer *>(graphicBuffer)->bind();
        glBindTexture(tar, GL_NONE);
    } else {
        texCompat->update(buf, w, h);
    }
}

bool HwAndroidTexture::support() {
    return sdk >= 26 || sdk <= 23;
}

bool HwAndroidTexture::read(uint8_t *pixels) {
    if (graphicBuffer) {
        return static_cast<HwAbsGraphicBuffer *>(graphicBuffer)->read(pixels);
    }
    return texCompat->read(pixels);
}
