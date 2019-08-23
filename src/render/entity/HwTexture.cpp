/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwTexture.h"
#include "../include/Egl.h"
#include "Logcat.h"
#include <GLES2/gl2.h>

#ifdef __ANDROID__

#include <GLES2/gl2ext.h>

HwTexture *HwTexture::allocOES() {
    return alloc(GL_TEXTURE_EXTERNAL_OES);
}

#endif

HwTexture *HwTexture::alloc() {
    return alloc(GL_TEXTURE_2D);
}

HwTexture *HwTexture::alloc(uint32_t target) {
    if (EGL_NO_CONTEXT == Egl::currentContext()) {
        Logcat::e("hwvc", "Please attach an egl context first.");
        return nullptr;
    }
    return new HwTexture(target);
}

HwTexture *HwTexture::wrap(uint32_t target, uint32_t tex, int w, int h) {
    return new HwTexture(target, tex, w, h);
}

HwTexture::HwTexture(uint32_t target) : Object(),
                                        tar(target),
                                        tex(GL_NONE),
                                        fmt(GL_RGBA),
                                        size(Size(0, 0)),
                                        isRef(false) {
    glGenTextures(1, &tex);
    bind();
    glTexParameterf(tar, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(tar, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(tar, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(tar, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    unbind();
}

HwTexture::HwTexture(uint32_t target, uint32_t tex, int w, int h) : Object(),
                                                                    tar(target),
                                                                    tex(tex),
                                                                    fmt(GL_RGBA),
                                                                    size(Size(w, h)),
                                                                    isRef(true) {
}

HwTexture::~HwTexture() {
    if (isRef) {
        tex = 0;
    } else {
        if (GL_NONE != tex) {
            glDeleteTextures(1, &tex);
        }
    }
    size.width = 0;
    size.height = 0;
}

void HwTexture::bind() {
    glBindTexture(tar, tex);
}

void HwTexture::unbind() {
    glBindTexture(tar, GL_NONE);
}

uint32_t HwTexture::texId() {
    return tex;
}

int HwTexture::getWidth() {
    return size.width;
}

int HwTexture::getHeight() {
    return size.height;
}

uint32_t HwTexture::target() { return tar; }

void HwTexture::update(HwBuffer *buf, int32_t w, int32_t h) {
    size.width = w;
    size.height = h;
    bind();
    if (applied && buf) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, fmt, GL_UNSIGNED_BYTE, buf->data());
    } else {
        if (buf) {
            glTexImage2D(tar, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, buf->data());
        } else {
            glTexImage2D(tar, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, nullptr);
        }
    }
    unbind();
}