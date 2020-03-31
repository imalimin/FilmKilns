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
#include "HwFBObject.h"
#include "AlMath.h"
#include "HwAbsTexture.h"

#define TAG "HwCameraInput"

HwCameraInput::HwCameraInput(string alias) : Unit(alias), srcTex(nullptr) {
    registerEvent(EVENT_CAMERA_INVALIDATE,
                  reinterpret_cast<EventFunc>(&HwCameraInput::eventInvalidate));
    registerEvent(MSG_CAMERA_UPDATE_SIZE,
                  reinterpret_cast<EventFunc>(&HwCameraInput::_onUpdateSize));
}

HwCameraInput::~HwCameraInput() {

}

bool HwCameraInput::onCreate(AlMessage *msg) {
    Logcat::i("HWVC", "HwCameraInput::eventPrepare");
    egl = AlEgl::offScreen(AlEgl::currentContext());
    srcTex = HwTexture::allocOES();
    AlTexDescription desc;
    destTex = HwTexture::alloc(desc);
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
    auto *m = AlMessage::obtain(MSG_CAMERA_OES_TEX_NOTIFY);
    m->ptr = srcTex.as<Object>();
    postMessage(m);
    return true;
}

bool HwCameraInput::onDestroy(AlMessage *msg) {
    Logcat::i("HWVC", "HwCameraInput::onDestroy");
    egl->makeCurrent();
    if (fbo) {
        delete fbo;
        fbo = nullptr;
    }
    srcTex.release();
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

bool HwCameraInput::eventInvalidate(AlMessage *msg) {
    int64_t tsInNs = msg->arg2;
    int width = getInt32("width");
    int height = getInt32("height");
    if (msg->obj) {
        AlMatrix *m = msg->getObj<AlMatrix *>();
        updateMatrix(width, height, m);
    }
    draw(width, height);
    notify(tsInNs, width, height);
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
    program->draw(srcTex.as());
    fbo->unbind();
}

void HwCameraInput::notify(int64_t tsInNs, int w, int h) {
    AlMessage *msg = AlMessage::obtain(EVENT_RENDER_FILTER);
    msg->obj = HwTexture::wrap(destTex);
    msg->arg2 = tsInNs;
    msg->queueMode = AlMessage::QUEUE_MODE_UNIQUE;
    postEvent(msg);
}

uint32_t HwCameraInput::getTex() {
    if (!srcTex.isNull()) {
        return srcTex->texId();
    }
    return 0;
}

void HwCameraInput::mackCurrent() {
    if (egl) {
        egl->makeCurrent();
    }
}

void HwCameraInput::updateMatrix(int32_t w, int32_t h, AlMatrix *matrix) {
    AlMatrix scale;
    float vRatio = w / (float) h;
    float cRatio = cameraSize.ratio();
    if (cRatio > vRatio) {
        scale.setScale(vRatio / cRatio, -1.0f);
    } else {
        scale.setScale(1.0f, -cRatio / vRatio);
    }
    AlMatrix trans = scale * (*matrix);
    program->updateMatrix(&trans);
}

void HwCameraInput::_onUpdateSize(AlMessage *msg) {
    cameraSize.width = msg->ptr.as<AlSize>()->width;
    cameraSize.height = msg->ptr.as<AlSize>()->height;
    AlLogI(TAG, "%dx%d", cameraSize.width, cameraSize.height);
}
