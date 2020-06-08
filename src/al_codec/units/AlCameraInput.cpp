/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlCameraInput.h"
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
#include <GLES2/gl2.h>

#define TAG "AlCameraInput"

AlCameraInput::AlCameraInput(string alias) : Unit(alias), srcTex(nullptr) {
    al_reg_msg(EVENT_CAMERA_INVALIDATE, AlCameraInput::_onInvalidate);
    al_reg_msg(MSG_CAMERA_UPDATE_SIZE, AlCameraInput::_onUpdateSize);
    al_reg_msg(MSG_CAMERA_RUN, AlCameraInput::_onRun);
    al_reg_msg(EVENT_LAYER_QUERY_ID_NOTIFY, AlCameraInput::_onLayerNotify);
    al_reg_msg(MSG_VIDEO_OUTPUT_SIZE, AlCameraInput::_onOutputSize);
}

AlCameraInput::~AlCameraInput() {

}

bool AlCameraInput::onCreate(AlMessage *msg) {
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
    m->obj = HwTexture::wrap(srcTex);
    postMessage(m);
    return true;
}

bool AlCameraInput::onDestroy(AlMessage *msg) {
    AlLogI(TAG, "");
    delete srcTex;
    srcTex = nullptr;
    delete fbo;
    fbo = nullptr;
    if (program) {
        delete program;
        program = nullptr;
    }
    return true;
}

bool AlCameraInput::_onInvalidate(AlMessage *msg) {
    if (nullptr == mLayerTex) {
        if (!fbo) {
            auto *m = AlMessage::obtain(MSG_LAYER_ADD_EMPTY,
                                        new AlSize(cameraSize.width, cameraSize.height));
            postMessage(m);
            fbo = HwFBObject::alloc();
        }
        return true;
    }
    AlLogD(TAG, "%dx%d, %dx%d", mLayerTex->getWidth(), mLayerTex->getHeight(),
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

void AlCameraInput::draw() {
    if (mLayerTex) {
//        AlLogI(TAG, "%d, %d", mLayerId, mLayerTex->texId());
        glViewport(0, 0, mLayerTex->getWidth(), mLayerTex->getHeight());
        fbo->bindTex(mLayerTex);
        fbo->bind();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
        program->draw(srcTex);
        fbo->unbind();
    }
}

void AlCameraInput::notify(int64_t tsInNs) {
    auto *m = AlMessage::obtain(MSG_TIMESTAMP);
    m->arg2 = tsInNs;
    postMessage(m);

    AlMessage *msg = AlMessage::obtain(EVENT_COMMON_INVALIDATE);
    AlRenderParams params;
    params.setTransparent(true);
    msg->arg1 = params.toInt();
    postEvent(msg);
}

void AlCameraInput::updateMatrix(int32_t w, int32_t h, AlMatrix *matrix) {
    program->updateMatrix(matrix);
}

bool AlCameraInput::_onUpdateSize(AlMessage *msg) {
    auto *size = msg->getObj<AlSize *>();
    if (size) {
        cameraSize.width = size->width;
        cameraSize.height = size->height;
    }
//    AlLogI(TAG, "%dx%d", cameraSize.width, cameraSize.height);
    return true;
}

bool AlCameraInput::_onRun(AlMessage *msg) {
    auto *func = msg->getObj<AlRunnable *>();
    if (func) {
        (*func)(nullptr);
    }
    return true;
}

bool AlCameraInput::_onLayerNotify(AlMessage *msg) {
    mLayerId = msg->arg1;
    if (nullptr == mLayerTex) {
        mLayerTex = HwTexture::wrap(msg->getObj<HwAbsTexture *>());
        auto scaleX = AlRational(mLayerTex->getWidth(), cameraSize.width);
        auto scaleY = AlRational(mLayerTex->getHeight(), cameraSize.height);
        if (scaleY.toFloat() > scaleX.toFloat()) {
            scaleX = scaleY;
        }
        auto *desc = new AlOperateScale(mLayerId, scaleX, AlVec2(0, 0));
        /// Y轴镜像
        desc->scaleY.num = -desc->scaleY.num;
        postEvent(AlMessage::obtain(EVENT_LAYER_SCALE, desc));
    }
    AlLogI(TAG, "%d", mLayerId);
    return true;
}

bool AlCameraInput::_onOutputSize(AlMessage *msg) {
    auto size = msg->getObj<AlSize *>();
    if (size) {
        outSize.width = size->width;
        outSize.height = size->height;
        AlLogI(TAG, "%dx%d", outSize.width, outSize.height);
    }
    return true;
}
