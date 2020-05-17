/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/AlUOESTexInput.h"
#include <GLES2/gl2.h>
#include "Egl.h"
#include "NativeWindow.h"
#include "HwFBObject.h"
#include "AlMath.h"
#include "HwAbsTexture.h"
#include "ObjectBox.h"
#include "AlRunnable.h"
#include "AlTexManager.h"
#include "AlRenderParams.h"
#include "AlOperateScale.h"

#define TAG "AlUOESTexInput"

AlUOESTexInput::AlUOESTexInput(string alias) : Unit(alias), srcTex(nullptr) {
    registerEvent(EVENT_CAMERA_INVALIDATE,
                  reinterpret_cast<EventFunc>(&AlUOESTexInput::_onInvalidate));
    registerEvent(MSG_CAMERA_UPDATE_SIZE,
                  reinterpret_cast<EventFunc>(&AlUOESTexInput::_onUpdateSize));
    registerEvent(MSG_CAMERA_RUN,
                  reinterpret_cast<EventFunc>(&AlUOESTexInput::_onRun));
    registerEvent(EVENT_LAYER_QUERY_ID_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlUOESTexInput::_onLayerNotify));
    registerEvent(MSG_VIDEO_OUTPUT_SIZE,
                  reinterpret_cast<EventFunc>(&AlUOESTexInput::_onOutputSize));
}

AlUOESTexInput::~AlUOESTexInput() {

}

bool AlUOESTexInput::onCreate(AlMessage *msg) {
    AlLogI(TAG, "");
    srcTex = HwTexture::allocOES();
    string vertex(R"(
        attribute vec4 aPosition;
        attribute vec4 aTextureCoord;
        uniform mat4 uTextureMatrix;
        varying vec2 vTextureCoord;
        void main() {
            gl_Position = aPosition;
            vTextureCoord = (uTextureMatrix * aTextureCoord).xy;
        })");
    string fragment(R"(
        #extension GL_OES_EGL_image_external : require
        precision mediump float;
        varying mediump vec2 vTextureCoord;
        uniform samplerExternalOES uTexture;
        void main() {
            vec4 color = vec4(texture2D(uTexture, vTextureCoord).rgb, 1.0);
            gl_FragColor = color;
        })");
    program = HwProgram::create(&vertex, &fragment);
    auto *m = AlMessage::obtain(MSG_CAMERA_OES_TEX_NOTIFY);
    m->ptr = srcTex.as<Object>();
    postMessage(m);
    return true;
}

bool AlUOESTexInput::onDestroy(AlMessage *msg) {
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

bool AlUOESTexInput::_onInvalidate(AlMessage *msg) {
    if (nullptr == mLayerTex) {
        if (!fbo) {
            _createLayer();
            fbo = HwFBObject::alloc();
        }
        return true;
    }
    AlLogI(TAG, "%dx%d, %dx%d", mLayerTex->getWidth(), mLayerTex->getHeight(),
           cameraSize.width, cameraSize.height);
    int64_t tsInNs = msg->arg2;
    if (msg->obj) {
        AlMatrix *m = msg->getObj<AlMatrix *>();
        updateMatrix(mLayerTex->getWidth(), mLayerTex->getHeight(), m);
    }
    draw();
    notify(tsInNs);
    return true;
}

void AlUOESTexInput::_createLayer() {
    auto *m = AlMessage::obtain(MSG_LAYER_ADD_EMPTY,
                                new AlSize(cameraSize.width, cameraSize.height));
    postMessage(m);
}

void AlUOESTexInput::draw() {
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

void AlUOESTexInput::notify(int64_t tsInNs) {
    AlMessage *msg = AlMessage::obtain(EVENT_COMMON_INVALIDATE, nullptr,
                                       AlMessage::QUEUE_MODE_UNIQUE);
    AlRenderParams params;
    params.setTransparent(true);
    msg->arg1 = params.toInt();
    postEvent(msg);

    auto *m = AlMessage::obtain(MSG_TIMESTAMP);
    m->arg2 = tsInNs;
    postMessage(m);
}

void AlUOESTexInput::updateMatrix(int32_t w, int32_t h, AlMatrix *matrix) {
    program->updateMatrix(matrix);
}

bool AlUOESTexInput::_onUpdateSize(AlMessage *msg) {
    cameraSize.width = msg->ptr.as<AlSize>()->width;
    cameraSize.height = msg->ptr.as<AlSize>()->height;
//    AlLogI(TAG, "%dx%d", cameraSize.width, cameraSize.height);
    return true;
}

bool AlUOESTexInput::_onRun(AlMessage *msg) {
    auto *func = msg->getObj<AlRunnable *>();
    if (func) {
        (*func)(nullptr);
    }
    return true;
}

bool AlUOESTexInput::_onLayerNotify(AlMessage *msg) {
    mLayerId = msg->arg1;
    if (nullptr == mLayerTex) {
        mLayerTex = HwTexture::wrap(msg->getObj<HwAbsTexture *>());
        auto *desc = new AlOperateScale(mLayerId, AlRational(1, 1), AlVec2(0, 0));
        /// Y轴镜像
        desc->scaleY.num = -desc->scaleY.num;
        postEvent(AlMessage::obtain(EVENT_LAYER_SCALE, desc));
    }
    AlLogI(TAG, "%d", mLayerId);
    return true;
}

bool AlUOESTexInput::_onOutputSize(AlMessage *msg) {
    auto size = msg->getObj<AlSize *>();
    if (size) {
        outSize.width = size->width;
        outSize.height = size->height;
        AlLogI(TAG, "%dx%d", outSize.width, outSize.height);
    }
    return true;
}
