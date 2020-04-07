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
#include "ObjectBox.h"
#include "AlRunnable.h"
#include "AlTexManager.h"

#define TAG "HwCameraInput"

HwCameraInput::HwCameraInput(string alias) : Unit(alias), srcTex(nullptr) {
    registerEvent(EVENT_CAMERA_INVALIDATE,
                  reinterpret_cast<EventFunc>(&HwCameraInput::_onInvalidate));
    registerEvent(MSG_CAMERA_UPDATE_SIZE,
                  reinterpret_cast<EventFunc>(&HwCameraInput::_onUpdateSize));
    registerEvent(MSG_CAMERA_RUN,
                  reinterpret_cast<EventFunc>(&HwCameraInput::_onRun));
    registerEvent(EVENT_LAYER_QUERY_ID_NOTIFY,
                  reinterpret_cast<EventFunc>(&HwCameraInput::_onLayerNotify));
}

HwCameraInput::~HwCameraInput() {

}

bool HwCameraInput::onCreate(AlMessage *msg) {
    AlLogI(TAG, "");
    srcTex = HwTexture::allocOES();
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
    AlLogI(TAG, "");
    srcTex.release();
    delete fbo;
    fbo = nullptr;
    if (program) {
        delete program;
        program = nullptr;
    }
    return true;
}

bool HwCameraInput::_onInvalidate(AlMessage *msg) {
    AlLogI(TAG, "");
    int64_t tsInNs = msg->arg2;
    if (msg->obj) {
        AlMatrix *m = msg->getObj<AlMatrix *>();
        updateMatrix(cameraSize.width, cameraSize.height, m);
    }
    draw(cameraSize.width, cameraSize.height);
    notify(tsInNs, cameraSize.width, cameraSize.height);
    return true;
}

void HwCameraInput::draw(int w, int h) {
    if (!fbo) {
        auto *m = AlMessage::obtain(MSG_LAYER_ADD_EMPTY, new AlSize(w, h));
        postMessage(m);
        fbo = HwFBObject::alloc();
    }
    if (mLayerTex) {
//        AlLogI(TAG, "%d, %d", mLayerId, mLayerTex->texId());
        glViewport(0, 0, mLayerTex->getWidth(), mLayerTex->getHeight());
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        fbo->bindTex(mLayerTex);
        fbo->bind();
        program->draw(srcTex.as<HwAbsTexture>());
        fbo->unbind();
    }
}

void HwCameraInput::notify(int64_t tsInNs, int w, int h) {
    AlMessage *msg = AlMessage::obtain(EVENT_COMMON_INVALIDATE, nullptr,
                                       AlMessage::QUEUE_MODE_UNIQUE);
    msg->arg1 = 0;
    postEvent(msg);

    auto *m = AlMessage::obtain(MSG_TIMESTAMP);
    m->arg2 = tsInNs;
    postMessage(m);
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

bool HwCameraInput::_onUpdateSize(AlMessage *msg) {
    cameraSize.width = msg->ptr.as<AlSize>()->width;
    cameraSize.height = msg->ptr.as<AlSize>()->height;
//    AlLogI(TAG, "%dx%d", cameraSize.width, cameraSize.height);
    return true;
}

bool HwCameraInput::_onRun(AlMessage *msg) {
    auto *func = msg->getObj<AlRunnable *>();
    if (func) {
        (*func)(nullptr);
    }
    return true;
}

bool HwCameraInput::_onLayerNotify(AlMessage *msg) {
    mLayerId = msg->arg1;
    if (nullptr == mLayerTex) {
        mLayerTex = HwTexture::wrap(msg->getObj<HwAbsTexture *>());
    }
    AlLogI(TAG, "%d", mLayerId);
    return true;
}
