/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicTexture.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include "FkDefinition.h"

#define TAG "FkGraphicAllocator"

FkTexDescription::FkTexDescription() : FkObject() {

}

FkTexDescription::FkTexDescription(uint32_t target) : FkObject(), target(target) {

}

FkTexDescription::FkTexDescription(const FkTexDescription &o)
        : FkObject(o), target(o.target), wrapMode(o.wrapMode), size(o.size), fmt(o.fmt) {
    memcpy(borderColor, o.borderColor, sizeof(float) * 4);
}

FkTexDescription::~FkTexDescription() {

}

uint32_t FkGraphicTexture::convertGLFormat(FkColor::kFormat fmt) {
    switch (fmt) {
        case FkColor::kFormat::RGBA: {
            return GL_RGBA;
        }
        case FkColor::kFormat::RGB: {
            return GL_RGB;
        }
        case FkColor::kFormat::LUMINANCE: {
            return GL_LUMINANCE;
        }
        case FkColor::kFormat::RGB565: {
            return GL_RGB565;
        }
        default:
            return GL_NONE;
    }

}

FkGraphicTexture::FkGraphicTexture() : FkSource() {
    FK_MARK_SUPER
}

FkGraphicTexture::~FkGraphicTexture() {
}

void FkGraphicTexture::bind() {
    glBindTexture(desc.target, tex);
}

void FkGraphicTexture::unbind() {
    glBindTexture(desc.target, GL_NONE);
}

FkResult FkGraphicTexture::create() {
    return FK_OK;
}

void FkGraphicTexture::destroy() {
    desc.target = GL_NONE;
    desc.size.set(0, 0);
    if (GL_NONE != tex && glIsTexture(tex)) {
        glDeleteTextures(1, &tex);
        tex = GL_NONE;
    }
}

size_t FkGraphicTexture::size() {
    int byteCount = 0;
    switch (desc.fmt) {
        case FkColor::kFormat::RGBA: {
            byteCount = 4;
            break;
        }
        case FkColor::kFormat::RGB: {
            byteCount = 3;
            break;
        }
        case FkColor::kFormat::LUMINANCE: {
            byteCount = 1;
            break;
        }
        case FkColor::kFormat::RGB565: {
            byteCount = 2;
            break;
        }
        default:
            byteCount = 0;
    }
    return desc.size.getWidth() * desc.size.getHeight() * byteCount;
}

void FkGraphicTexture::update(FkColor::kFormat fmt, int32_t width, int32_t height) {
    bind();
    desc.fmt = fmt;
    desc.size.set(width, height);
    glTexImage2D(desc.target, 0, convertGLFormat(desc.fmt),
                 desc.size.getWidth(), desc.size.getHeight(), 0,
                 convertGLFormat(desc.fmt), GL_UNSIGNED_BYTE, nullptr);
    unbind();
}

FkGraphicAllocator::FkGraphicAllocator() : FkSourceAllocator<FkGraphicTexture, FkTexDescription>() {
    FK_MARK_SUPER
}

FkGraphicAllocator::~FkGraphicAllocator() {

}

std::shared_ptr<FkGraphicTexture> FkGraphicAllocator::delegateAlloc(FkTexDescription &desc) {
    if (EGL_NO_CONTEXT == eglGetCurrentContext()) {
        FkLogE(TAG, "Allocate texture failed. Invalid EGL context");
        return nullptr;
    }
    if (0 == desc.target) {
        FkLogE(TAG, "Allocate texture failed. Invalid target(%d)", desc.target);
    }
    std::shared_ptr<FkGraphicTexture> o(new FkGraphicTexture(), [this](FkGraphicTexture *o) {
        this->recycle(o);
    });
    o->create();
    o->desc = desc;
    glGenTextures(1, &(o->tex));
    o->bind();
    glTexParameterf(desc.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(desc.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    switch (desc.wrapMode) {
        case FkTexDescription::WrapMode::EDGE: {
            glTexParameterf(desc.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(desc.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        }
        case FkTexDescription::WrapMode::BORDER: {
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
    o->unbind();
    return o;
}
