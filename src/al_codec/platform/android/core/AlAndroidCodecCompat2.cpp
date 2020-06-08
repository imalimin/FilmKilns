/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAndroidCodecCompat2.h"
#include "AlTexFrame.h"

#define TAG "AlAndroidCodecCompat2"

AlAndroidCodecCompat2::AlAndroidCodecCompat2(AlCodec::kID id) : AlAndroidCodecCompat(id) {

}

AlAndroidCodecCompat2::~AlAndroidCodecCompat2() {
    delete egl;
    egl = nullptr;
}

HwResult AlAndroidCodecCompat2::configure(HwBundle &format) {
    AlCodec::configure(format);
    mExtraData = AlAndroidCodecCompat::makeExtraData(getCodecID(), format);
    int32_t width = format.getInt32(KEY_WIDTH);
    int32_t height = format.getInt32(KEY_HEIGHT);
    int32_t bitrate = (int32_t) format.getInt32(KEY_BIT_RATE);
    this->keyFrameBuf = HwBuffer::alloc(static_cast<size_t>(width * height * 3 / 2));

    if (AlCodec::kID::H264 == getCodecID()) {
        fps = format.getInt32(KEY_FPS);
        if (encodeMode) {
            bridge->configure(width, height, bitrate, COLOR_FormatSurface,
                              3, fps, CONFIGURE_FLAG_ENCODE);
        } else {
//            HwBuffer *csd0Buf = dynamic_cast<HwBuffer *>(format.getObject("csd-0"));
//            HwBuffer *csd1Buf = dynamic_cast<HwBuffer *>(format.getObject("csd-1"));
//            AMediaFormat_setBuffer(cf, "csd-0", csd0Buf->data(), csd0Buf->size());
//            AMediaFormat_setBuffer(cf, "csd-1", csd1Buf->data(), csd1Buf->size());
        }
    }
    auto *win = bridge->createInputSurface();
    egl = AlEgl::androidCodec(TAG, win, AlEgl::currentContext());
    if (nullptr == egl || Hw::SUCCESS != bridge->start()) {
        AlLogE(TAG, "codec start failed.");
        release();
        delete egl;
        egl = nullptr;
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

HwResult AlAndroidCodecCompat2::process(HwAbsMediaFrame **frame, HwPacket **pkt) {
    if (nullptr == frame || nullptr == *frame || !AL_INSTANCE_OF(*frame, AlTexFrame *)) {
        return Hw::FAILED;
    }
    if (nullptr == program) {
        createProgram();
    }
    AlTexFrame *tFrame = dynamic_cast<AlTexFrame *>(*frame);
    uint8_t *data = nullptr;
    size_t size = 0;
    int64_t pts = tFrame->getPts();

    egl->makeCurrent();
    glViewport(0, 0, egl->width(), egl->height());
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program->draw(tFrame->tex());
    egl->setTimestamp(pts * 1000);
    egl->swapBuffers();

    HwResult ret1 = pop(2000);
    if (Hw::SUCCESS == ret1) {
        if (encodeMode) {
            *pkt = hwPacket;
        } else {
            *frame = outFrame;
        }
    } else if (Hw::IO_EOF == ret1) {
        return ret1;
    }
    return ret1;
}

bool AlAndroidCodecCompat2::createProgram() {
    string vertex(R"(
        attribute vec4 aPosition;
        attribute vec2 aTextureCoord;
        varying vec2 vTextureCoord;
        uniform mat4 uTextureMatrix;
        void main(){
            gl_Position= uTextureMatrix * aPosition;
            vTextureCoord = aTextureCoord;
        })");
    string fragment(R"(
        precision mediump float;
        varying mediump vec2 vTextureCoord;
        uniform sampler2D uTexture;
        void main(){
            vec4 color = vec4(texture2D(uTexture, vTextureCoord).rgb, 1.0);
            gl_FragColor = color;
        })");
    program = HwProgram::create(&vertex, &fragment);
    auto *mat = new AlMatrix();
    mat->setScale(1, -1);
    static_cast<HwProgram *>(program)->updateMatrix(mat);
    delete mat;
    return nullptr != program;
}
