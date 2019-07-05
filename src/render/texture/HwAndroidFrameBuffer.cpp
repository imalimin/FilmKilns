/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwAndroidFrameBuffer.h"
#include "HwAHardwareBuffer.h"
#include "HwAndroidGraphicBuffer.h"
#include "HwAndroidUtils.h"
#include "Logcat.h"
//#include <media/NdkImageReader.h>

HwAndroidFrameBuffer::HwAndroidFrameBuffer(int w, int h) : HwAbsFrameBuffer(w, h) {
    sdk = HwAndroidUtils::getAndroidApi();
    createTexture();
}

HwAndroidFrameBuffer::~HwAndroidFrameBuffer() {
    if (graphicBuffer) {
        delete graphicBuffer;
        graphicBuffer = nullptr;
    }
    if (frameBuffer) {
        delete frameBuffer;
        frameBuffer = nullptr;
    }
}

GLuint HwAndroidFrameBuffer::getFrameBuffer() {
    if (frameBuffer) {
        return frameBuffer->getFrameBuffer();
    } else {
        return HwAbsFrameBuffer::getFrameBuffer();
    }
}

GLuint HwAndroidFrameBuffer::getFrameTexture() {
    if (frameBuffer) {
        return frameBuffer->getFrameTexture();
    } else {
        return HwAbsFrameBuffer::getFrameTexture();
    }
}

bool HwAndroidFrameBuffer::read(uint8_t *pixels) {
    if (frameBuffer) {
        return frameBuffer->read(pixels);
    }
    if (graphicBuffer) {
        return static_cast<HwAbsGraphicBuffer *>(graphicBuffer)->read(pixels);
    }
    return false;
}

void HwAndroidFrameBuffer::createTexture() {
    /**
     * Support GraphicBuffer.
     * sdk >= 26: AHardwareBuffer.
     * sdk == 24 || sdk == 25: Normal.
     * sdk <= 23: GraphicBuffer.
     */
    if (sdk >= 26 || sdk <= 23) {
        glGenTextures(1, &id);
        glGenFramebuffers(1, &fbo);
        glBindTexture(GL_TEXTURE_2D, id);
        if (sdk >= 26) {
            graphicBuffer = new HwAHardwareBuffer(width(), height());
        } else if (sdk <= 23) {
            graphicBuffer = new HwAndroidGraphicBuffer(width(), height());
        } else {
            return;
        }
        static_cast<HwAbsGraphicBuffer *>(graphicBuffer)->bind();
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D,
                        GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, id, 0);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    } else {
        frameBuffer = new HwFrameBuffer(width(), height());
    }
}