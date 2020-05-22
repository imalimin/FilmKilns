/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUTexReader.h"
#include "AlTexManager.h"
#include "AlRotateFilter.h"
#include "HwRGBA2NV12Filter.h"
#include "HwFBObject.h"
#include "HwTexture.h"
#include "AlBitmapFactory.h"
#include "AlColorFormat.h"

AlUTexReader::AlUTexReader(const string &alias) : Unit(alias) {
    al_reg_msg(EVENT_SCREEN_DRAW_TEX, AlUTexReader::_onScreenDraw);
    al_reg_msg(MSG_TEX_READER_REQ_PIXELS, AlUTexReader::_onReqPixels);
}

AlUTexReader::~AlUTexReader() {
}

bool AlUTexReader::onCreate(AlMessage *msg) {
    return true;
}

bool AlUTexReader::onDestroy(AlMessage *msg) {
    delete resizeFilter;
    resizeFilter = nullptr;
    delete yuvFilter;
    yuvFilter = nullptr;
    AlTexManager::instance()->recycle(&resizeTex);
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

#if 0
    size_t size = static_cast<size_t>(srcTex->getWidth() * srcTex->getHeight() * 4);
    AlBuffer *buf = AlBuffer::alloc(size);
    if (nullptr == fbo) {
        fbo = HwFBObject::alloc();
    }
    glFinish();
    fbo->bindTex(srcTex);
    fbo->bind();
    srcTex->read(buf->data());
    fbo->unbind();
    AlBitmapFactory::save(srcTex->getWidth(), srcTex->getHeight(), buf, "/sdcard/000000.bmp");
    delete buf;
#endif
    return true;
}

bool AlUTexReader::_onReqPixels(AlMessage *msg) {
    if (nullptr == srcTex) {
        return true;
    }
    auto *srcTex = _resize(this->srcTex, msg->getObj<AlSize *>());
    auto color = static_cast<AlColorFormat >(msg->arg1);
    switch (color) {
        case AlColorFormat::NV12: {
            _yv12(srcTex);
            break;
        }
        case AlColorFormat::RGBA:
        default: {
            _rgba(srcTex);
            break;
        }
    }
    return true;
}

void AlUTexReader::_rgba(HwAbsTexture *srcTex) {
    const int size = srcTex->getWidth() * srcTex->getHeight() * 4;
    if (nullptr == pixels || pixels->capacity() < size) {
        delete pixels;
        pixels = AlBuffer::alloc(size);
    }
    pixels->rewind();
    pixels->limit(size);

    auto *m = AlMessage::obtain(MSG_TEX_READER_NOTIFY_PIXELS);
    glFinish();
    fbo->bindTex(srcTex);
    fbo->bind();
    if (fbo->read(pixels)) {
        m->obj = AlBuffer::wrap(pixels);
    }
    fbo->unbind();
    postMessage(m);
}

void AlUTexReader::_yv12(HwAbsTexture *srcTex) {
    if (nullptr == yuvFilter) {
        yuvFilter = new HwRGBA2NV12Filter();
        bool ret = yuvFilter->prepare();
        AlTexDescription desc;
        desc.size.width = srcTex->getWidth() / 4;
        desc.size.height = srcTex->getHeight() * 3 / 2;
        yuvTex = AlTexManager::instance()->alloc(desc);
        fbo = HwFBObject::alloc();
    }
    glViewport(0, 0, yuvTex->getWidth(), yuvTex->getHeight());
    yuvFilter->draw(srcTex, yuvTex);

    const int size = yuvTex->getWidth() * yuvTex->getHeight() * 4;
    if (nullptr == pixels || pixels->capacity() < size) {
        delete pixels;
        pixels = AlBuffer::alloc(size);
    }
    pixels->rewind();
    pixels->limit(size);

    auto *m = AlMessage::obtain(MSG_TEX_READER_NOTIFY_PIXELS);
    glFinish();
    fbo->bindTex(yuvTex);
    fbo->bind();
    if (fbo->read(pixels)) {
        m->obj = AlBuffer::wrap(pixels);
    }
    fbo->unbind();
    postMessage(m);
}

HwAbsTexture *AlUTexReader::_resize(HwAbsTexture *srcTex, AlSize *size) {
    if (size && size->width != srcTex->getWidth() && size->height != srcTex->getHeight()) {
        if (nullptr == resizeFilter) {
            resizeFilter = new AlRotateFilter();
            resizeFilter->prepare();
        }
        if (nullptr == resizeTex) {
            AlTexDescription desc;
            desc.size.width = size->width;
            desc.size.height = size->height;
            resizeTex = AlTexManager::instance()->alloc(desc);
        }
        glViewport(0, 0, resizeTex->getWidth(), resizeTex->getHeight());
        resizeFilter->draw(srcTex, resizeTex);
        return resizeTex;
    }
    return srcTex;
}
