/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicTexture.h"
#include "FkGLDefinition.h"

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

FkGraphicTexture::FkGraphicTexture(const FkTexDescription &desc)
        : FkSource(), desc(desc), applied(false) {
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
    FkAssert(GL_NONE != desc.target, FK_FAIL);
    FkAssert(desc.fmt != FkColor::kFormat::NONE, FK_FAIL);
    glGenTextures(1, &tex);
    bind();
    glTexParameterf(desc.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(desc.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    switch (desc.wrapMode) {
        case FkTexDescription::kWrapMode::EDGE: {
            glTexParameterf(desc.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(desc.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        }
        case FkTexDescription::kWrapMode::BORDER: {
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
    return GL_NO_ERROR == glGetError() ? FK_OK : FK_FAIL;
}

void FkGraphicTexture::destroy() {
    desc.target = GL_NONE;
    desc.size.set(0, 0);
#ifdef __FK_DEBUG__
    FkAssert(EGL_NO_CONTEXT != eglGetCurrentContext(), );
#endif
    FkAssert(GL_NONE != tex, );
    FkAssert(glIsTexture(tex), );
    glDeleteTextures(1, &tex);
    tex = GL_NONE;
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

void FkGraphicTexture::update(FkColor::kFormat fmt, int32_t width, int32_t height, uint8_t *pixels) {
    FkAssert(fmt != FkColor::kFormat::NONE, );
    if (width != desc.size.getWidth() || height != desc.size.getHeight()) {
        applied = false;
    }
    desc.fmt = fmt;
    desc.size.set(width, height);
    bind();
    if (applied) {
        FK_GL_CHECK(glTexSubImage2D(desc.target, 0, 0, 0,
                        desc.size.getWidth(), desc.size.getHeight(),
                        convertGLFormat(desc.fmt), GL_UNSIGNED_BYTE, pixels));
    } else {
        applied = true;
        FK_GL_CHECK(glTexImage2D(desc.target, 0, convertGLFormat(desc.fmt),
                     desc.size.getWidth(), desc.size.getHeight(), 0,
                     convertGLFormat(desc.fmt), GL_UNSIGNED_BYTE, pixels));

    }
    unbind();
}

FkGraphicAllocator::FkGraphicAllocator() : FkSourceAllocator<FkGraphicTexture, FkTexDescription>() {
    FK_MARK_SUPER
}

FkGraphicAllocator::~FkGraphicAllocator() {

}

FkGraphicTexture *FkGraphicAllocator::delegateAlloc(FkTexDescription &desc) {
    if (EGL_NO_CONTEXT == eglGetCurrentContext()) {
        FkLogE(TAG, "Allocate texture failed. Invalid EGL context");
        return nullptr;
    }
    if (0 == desc.target) {
        FkLogE(TAG, "Allocate texture failed. Invalid target(%d)", desc.target);
    }
    auto o = new FkGraphicTexture(desc);
    if (FK_OK != o->create()) {
        delete o;
        return nullptr;
    }
    if (FkColor::kFormat::NONE != o->desc.fmt
        && 0 != o->desc.size.getWidth()
        && 0 != o->desc.size.getHeight()) {
        o->update(o->desc.fmt, o->desc.size.getWidth(), o->desc.size.getHeight());
    }
    return o;
}

bool FkGraphicAllocator::delegateEquals(FkTexDescription &desc, FkGraphicTexture *tex) {
    if (tex->desc.target == desc.target) {
        if (FkColor::kFormat::NONE != desc.fmt) {
            if (0 != desc.size.getWidth() && 0 != desc.size.getHeight()) {
                return desc.fmt == tex->desc.fmt
                       && tex->desc.size.getWidth() == desc.size.getWidth()
                       && tex->desc.size.getHeight() == desc.size.getHeight();
            }
//            return desc.fmt == tex->desc.fmt;
        }
    }
    return false;
}
