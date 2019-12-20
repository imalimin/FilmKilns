/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwTexture.h"
#include "../include/Egl.h"
#include "Logcat.h"

#ifdef __ANDROID__

#include <GLES2/gl2ext.h>

HwTexture *HwTexture::allocOES() {
    AlTexDescription desc;
    desc.target = GL_TEXTURE_EXTERNAL_OES;
    return alloc(desc);
}

#endif

#define TAG "HwTexture"

HwTexture *HwTexture::alloc(AlTexDescription &desc) {
    if (EGL_NO_CONTEXT == Egl::currentContext()) {
        Logcat::e(TAG, "Please attach an egl context first.");
        return nullptr;
    }
    return new HwTexture(desc);
}

HwTexture *HwTexture::wrap(HwTexture *tex) {
    HwTexture *t = new HwTexture(tex->desc);
    t->tex = tex->tex;
    t->isRef = true;
    return t;
}

HwTexture::HwTexture(AlTexDescription &desc) : HwAbsTexture(desc),
                                               isRef(false) {
    glGenTextures(1, &tex);
    bind();
    glTexParameterf(desc.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(desc.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    switch (desc.wrapMode) {
        case AlTexDescription::WrapMode::EDGE: {
            glTexParameterf(desc.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(desc.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        }
        case AlTexDescription::WrapMode::BORDER: {
            glTexParameterf(desc.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_EXT);
            glTexParameterf(desc.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_EXT);
            float color[] = {1.0f, 1.0f, 1.0f, 0.0f};
            if (GL_TEXTURE_2D == desc.target) {
                glTexParameterfv(desc.target, GL_TEXTURE_BORDER_COLOR_EXT, color);
            }
#ifdef __ANDROID__
            if (GL_TEXTURE_EXTERNAL_OES == desc.target) {
                glTexParameterfv(desc.target, GL_TEXTURE_BORDER_COLOR_OES, color);
            }
#endif
            break;
        }
        default:
            glTexParameterf(desc.target, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf(desc.target, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
    }
    unbind();
}

HwTexture::~HwTexture() {
    if (isRef) {
        tex = 0;
    } else {
        if (GL_NONE != tex && glIsTexture(tex)) {
            glDeleteTextures(1, &tex);
        }
    }
}

void HwTexture::bind() {
    glBindTexture(desc.target, tex);
}

void HwTexture::unbind() {
    glBindTexture(desc.target, GL_NONE);
}

void HwTexture::update(AlBuffer *buf, int32_t w, int32_t h, uint32_t fmt) {
    HwAbsTexture::update(buf, w, h, fmt);
    bind();
    if (applied && buf && getWidth() == w && getHeight() == h && fmt == this->fmt()) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, fmt, GL_UNSIGNED_BYTE, buf->data());
    } else {
        applied = true;
        if (buf) {
            glTexImage2D(desc.target, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, buf->data());
        } else {
            glTexImage2D(desc.target, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, nullptr);
        }
    }
    unbind();
}

bool HwTexture::read(uint8_t *pixels) {
    glReadPixels(0, 0, getWidth(), getHeight(), fmt(), GL_UNSIGNED_BYTE, pixels);
    return true;
}