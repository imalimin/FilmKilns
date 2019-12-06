/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwCameraInput.h"
#include <GLES2/gl2.h>
#include "Egl.h"
#include "NativeWindow.h"
#include "../include/HwFBObject.h"

HwCameraInput::HwCameraInput(string alias) : Unit(alias) {
    registerEvent(EVENT_CAMERA_INVALIDATE,
                  reinterpret_cast<EventFunc>(&HwCameraInput::eventInvalidate));
}

HwCameraInput::~HwCameraInput() {

}

bool HwCameraInput::onCreate(Message *msg) {
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

bool HwCameraInput::onDestroy(Message *msg) {
    Logcat::i("HWVC", "HwCameraInput::onDestroy");
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
    int32_t size = msg->arg1;
    int64_t tsInNs = msg->arg2;
    int32_t h = size & 0xFFFF;
    int32_t w = size >> 16;
    if (msg->obj) {
        HwMatrix *matrix = dynamic_cast<HwMatrix *>(msg->obj);
        updateMatrix(w, h, matrix);
    }
    draw(w, h);
    notify(tsInNs, w, h);
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
    }
    glViewport(0, 0, destTex->getWidth(), destTex->getHeight());
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    fbo->bind();
    program->draw(srcTex);
    fbo->unbind();
}

void HwCameraInput::notify(int64_t tsInNs, int w, int h) {
    Message *msg = new Message(EVENT_RENDER_FILTER, nullptr);
    msg->obj = HwTexture::wrap(destTex->target(), destTex->texId(), w, h, destTex->fmt());
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

void HwCameraInput::updateMatrix(int32_t w, int32_t h, HwMatrix *matrix) {
    HwMatrix scale;
    float ratio = getInt32("camera_width") / (float) getInt32("camera_height");
    if (ratio > w / (float) h) {
        scale.scale(w / (h * ratio), 1.0f, 1.0f);
    } else {
        scale.scale(1.0f, h / (w / ratio), 1.0f);
    }
    HwMatrix trans;
    trans.rotate(0.0f, HwMatrix::PI, 0.0f);
    matrix->multiplyBy(&trans);
    scale.multiplyBy(matrix);
    program->updateMatrix(&scale);
}