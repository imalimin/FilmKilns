/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwCameraInput.h"
#include <GLES2/gl2.h>
#include "Egl.h"
#include "NativeWindow.h"
#include "../include/HwFBObject.h"

HwCameraInput::HwCameraInput(string alias) : Unit(alias) {
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwCameraInput::eventPrepare));
    registerEvent(EVENT_CAMERA_INVALIDATE,
                  reinterpret_cast<EventFunc>(&HwCameraInput::eventInvalidate));
}

HwCameraInput::~HwCameraInput() {

}

bool HwCameraInput::eventPrepare(Message *msg) {
    Logcat::i("HWVC", "HwCameraInput::eventPrepare");
    egl = new Egl();
    srcTex = HwTexture::allocOES();
    destTex = HwTexture::alloc();
    string vertex("        attribute vec4 aPosition;\n"
                  "        attribute vec4 aTextureCoord;\n"
                  "        uniform mat4 uTextureMatrix;\n"
                  "        varying vec2 vTextureCoord;\n"
                  "        void main() {\n"
                  "            gl_Position = aPosition;\n"
                  "            vTextureCoord = (uTextureMatrix * aTextureCoord).xy;\n"
                  "        }");
    string fragment("        #extension GL_OES_EGL_image_external : require\n"
                    "        precision mediump float;\n"
                    "        varying mediump vec2 vTextureCoord;\n"
                    "        uniform samplerExternalOES uTexture;\n"
                    "        void main() {\n"
                    "            vec4 color = vec4(texture2D(uTexture, vTextureCoord).rgb, 1.0);\n"
                    "            gl_FragColor = color;\n"
                    "        }");
    program = HwProgram::create(&vertex, &fragment);
    NativeWindow *nw = static_cast<NativeWindow *>(msg->tyrUnBox());
    nw->context = egl->getContext();
    return true;
}

bool HwCameraInput::eventRelease(Message *msg) {
    Logcat::i("HWVC", "HwCameraInput::eventRelease");
    egl->makeCurrent();
    if (fbo) {
        delete fbo;
        fbo = nullptr;
    }
    if (srcTex) {
        delete srcTex;
        srcTex = nullptr;
    }
    if (destTex) {
        delete destTex;
        destTex = nullptr;
    }
    if (program) {
        delete program;
        program = nullptr;
    }
    if (egl) {
        delete egl;
        egl = nullptr;
    }
    return true;
}

bool HwCameraInput::eventInvalidate(Message *msg) {
    int64_t tsInNs = 0;
    if (msg->obj) {
        HwMatrix *matrix = dynamic_cast<HwMatrix *>(msg->obj);
        program->updateMatrix(matrix);
    }
    draw(msg->arg1, static_cast<int>(msg->arg2));
    notify(tsInNs, msg->arg1, static_cast<int>(msg->arg2));
    return true;
}

void HwCameraInput::draw(int w, int h) {
    egl->makeCurrent();
    if (!fbo) {
        if (destTex) {
            destTex->update(nullptr, w, h);
        }
        fbo = HwFBObject::alloc();
        fbo->bindTex(destTex);
//        calculateBestLocation(w, h, getInt32("camera_width"), getInt32("camera_height"));
    }
    glViewport(0, 0, destTex->getWidth(), destTex->getHeight());
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    program->draw(srcTex, fbo);
}

void HwCameraInput::notify(int64_t tsInNs, int w, int h) {
    Message *msg = new Message(EVENT_RENDER_FILTER, nullptr);
    msg->obj = HwTexture::wrap(destTex->target(), destTex->texId(), w, h);
    msg->arg2 = tsInNs;
    msg->queueMode = Message::QUEUE_MODE_UNIQUE;
    postEvent(msg);
}

uint32_t HwCameraInput::getTex() {
    if (srcTex) {
        return srcTex->texId();
    }
    return 0;
}

void HwCameraInput::mackCurrent() {
    if (egl) {
        egl->makeCurrent();
    }
}

void HwCameraInput::calculateBestLocation(int32_t vw, int32_t vh, int32_t w, int32_t h) {
    int32_t previewWidth = w, previewHeight = h;
    float previewScale = previewWidth / (float) previewHeight;
    float videoScale = vw / (float) vh;
    int32_t destPreviewWidth = previewWidth;
    int32_t destPreviewHeight = previewHeight;
    if (previewScale > videoScale) {
        destPreviewWidth = static_cast<int32_t>(previewHeight * videoScale);
        if (0 != destPreviewWidth % 2) ++destPreviewWidth;
    } else {
        destPreviewHeight = static_cast<int32_t>(previewWidth / videoScale);
        if (0 != destPreviewHeight % 2) ++destPreviewHeight;
    }
    float left = (previewWidth - destPreviewWidth) / 2.0f / previewWidth;
    float right = 1.0f - left;
    float bottom = (previewHeight - destPreviewHeight) / 2.0f / previewHeight;
    float top = 1.0f - bottom;
    // xy
    float position[8]{
            -1.0f, -1.0f, //LEFT,BOTTOM
            1.0f, -1.0f, //RIGHT,BOTTOM
            -1.0f, 1.0f, //LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    // st
    float texCoordinate[8]{
            left, top, //LEFT,BOTTOM
            right, top, //RIGHT,BOTTOM
            left, bottom, //LEFT,TOP
            right, bottom//RIGHT,TOP
    };
    program->updateLocation(texCoordinate, position);
}