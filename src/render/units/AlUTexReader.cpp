/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUTexReader.h"
#include "AlTexManager.h"
#include "HwRGBA2NV12Filter.h"
#include "HwFBObject.h"
#include "HwTexture.h"
#include "AlBitmapFactory.h"

AlUTexReader::AlUTexReader(const string &alias) : Unit(alias) {
    registerEvent(EVENT_SCREEN_DRAW_TEX,
                  reinterpret_cast<EventFunc>(&AlUTexReader::_onScreenDraw));
    registerEvent(MSG_TEX_READER_REQ_PIXELS,
                  reinterpret_cast<EventFunc>(&AlUTexReader::_onReqPixels));
}

AlUTexReader::~AlUTexReader() {
}

bool AlUTexReader::onCreate(AlMessage *msg) {
    return true;
}

bool AlUTexReader::onDestroy(AlMessage *msg) {
    delete yuvFilter;
    yuvFilter = nullptr;
    AlTexManager::instance()->recycle(&yuvTex);
    delete fbo;
    fbo = nullptr;
    delete pixels;
    pixels = nullptr;
    return true;
}

bool AlUTexReader::_onScreenDraw(AlMessage *msg) {
    delete srcTex;
    srcTex = HwTexture::wrap(msg->getObj<HwAbsTexture *>());

#if 1
    size_t size = static_cast<size_t>(srcTex->getWidth() * srcTex->getHeight() * 4);
    AlBuffer *buf = AlBuffer::alloc(size);
    if (nullptr == fbo) {
        fbo = HwFBObject::alloc();
    }
    fbo->bindTex(srcTex);
    fbo->bind();
    srcTex->read(buf->data());
    fbo->unbind();
    glFinish();
    AlBitmapFactory::save(srcTex->getWidth(), srcTex->getHeight(), buf, "/sdcard/000000.bmp");
    delete buf;
#endif
    return true;
}

bool AlUTexReader::_onReqPixels(AlMessage *msg) {
    if (nullptr == srcTex) {
        return true;
    }
    int format = msg->arg1;
    switch (format) {
        case 3: {
            break;
        }
    }
    if (nullptr == yuvFilter) {
        yuvFilter = new HwRGBA2NV12Filter();
        bool ret = yuvFilter->prepare();
        AlTexDescription desc;
        desc.size.width = srcTex->getWidth() / 4;
        desc.size.height = srcTex->getHeight() * 3 / 2;
        yuvTex = AlTexManager::instance()->alloc(desc);
        fbo = HwFBObject::alloc();
        pixels = AlBuffer::alloc(yuvTex->getWidth() * yuvTex->getHeight() * 4);
    }
    glViewport(0, 0, yuvTex->getWidth(), yuvTex->getHeight());
    yuvFilter->draw(srcTex, yuvTex);

    auto *m = AlMessage::obtain(MSG_TEX_READER_NOTIFY_PIXELS);
    glFinish();
    fbo->bindTex(yuvTex);
    fbo->bind();
    if (fbo->read(pixels->data())) {
        m->obj = AlBuffer::wrap(pixels->data(), pixels->size());
    }
    fbo->unbind();
    postMessage(m);
    return true;
}
