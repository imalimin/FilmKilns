/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "Logcat.h"
#include "../include/HwRender.h"
#include "../include/NormalFilter.h"
#include "../include/ObjectBox.h"
#include "TimeUtils.h"
#include "../include/RGBA2YV12Filter.h"

HwRender::HwRender() : HwRender(nullptr) {
}

HwRender::HwRender(HandlerThread *handlerThread) : Unit(handlerThread) {
    name = __FUNCTION__;
#ifdef ANDROID
    filter = new NormalFilter(true);
#else
    filter = new NormalFilter();
#endif
    yuvReadFilter = new RGBA2YV12Filter();
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwRender::eventPrepare));
    registerEvent(EVENT_COMMON_PIXELS_READ,
                  reinterpret_cast<EventFunc>(&HwRender::eventReadPixels));
    registerEvent(EVENT_RENDER_FILTER, reinterpret_cast<EventFunc>(&HwRender::eventRenderFilter));
    registerEvent(EVENT_RENDER_SET_FILTER, reinterpret_cast<EventFunc>(&HwRender::eventSetFilter));
}

HwRender::~HwRender() {
}

bool HwRender::eventPrepare(Message *msg) {
    Logcat::i("HWVC", "Render::eventPrepare");
    return true;
}

bool HwRender::eventRelease(Message *msg) {
    Logcat::i("HWVC", "Render::eventRelease");
    post([this] {
        if (yuvReadFilter) {
            delete yuvReadFilter;
            yuvReadFilter = nullptr;
        }
        if (filter) {
            delete filter;
            Logcat::i("HWVC", "Render::eventRelease filter");
            filter = nullptr;
        }
    });
    if (pixels) {
        delete[] pixels;
        pixels = nullptr;
    }
    if (buf) {
        delete buf;
        buf = nullptr;
    }
    return true;
}

bool HwRender::eventReadPixels(Message *msg) {
    bool read = false;
    if (yuvReadFilter) {
        yuvReadFilter->draw(filter->getFrameBuffer()->getFrameTexture());
        if (yuvReadFilter->getFrameBuffer()->read(buf->getData())) {
            read = true;
        }
    }
    if (!read && filter->getFrameBuffer()->read(buf->getData())) {
        read = true;
    }
    if (read) {
        Message *msg1 = new Message(EVENT_COMMON_PIXELS, nullptr);
        msg1->obj = HwBuffer::wrap(buf->getData(), buf->size());
        msg1->arg2 = tsInNs;
        postEvent(msg1);
    }
    return true;
}

bool HwRender::eventRenderFilter(Message *msg) {
    Logcat::i("HWVC", "Render::eventFilter");
    Size *size = static_cast<Size *>(msg->tyrUnBox());
    GLuint tex = msg->arg1;
    tsInNs = msg->arg2;
    post([this, size, tex] {
        checkFilter(size->width, size->height);
        glViewport(0, 0, size->width, size->height);
        renderFilter(tex);
        notifyPixelsReady();
        renderScreen();
        delete size;
    });
    return true;
}

bool HwRender::eventSetFilter(Message *msg) {
    Logcat::i("HWVC", "Render::eventSetFilter");
    Filter *newFilter = static_cast<Filter *>(msg->tyrUnBox());
    post([this, newFilter] {
        if (filter) {
            delete filter;
            filter = nullptr;
        }
        filter = newFilter;
    });
    return true;
}

void HwRender::renderScreen() {
    Logcat::i("HWVC", "Render::renderScreen");
    Message *msg = new Message(EVENT_SCREEN_DRAW, nullptr);
    msg->obj = new ObjectBox(new Size(filter->getFrameBuffer()->width(),
                                      filter->getFrameBuffer()->height()));
    msg->arg1 = filter->getFrameBuffer()->getFrameTexture();
    postEvent(msg);
}

void HwRender::checkFilter(int width, int height) {
    if (filter) {
        bool ret = filter->init(width, height);
        if (yuvReadFilter) {
            yuvReadFilter->init(width, height);
        }
        if (ret) {
            size_t size = static_cast<size_t>(filter->getFrameBuffer()->width()
                                              * filter->getFrameBuffer()->height() * 3 / 2);
            buf = HwBuffer::alloc(size);
        }
    }
    if (!pixels) {
        pixels = new uint8_t[width * height * 3 / 2];
    }
}

void HwRender::renderFilter(GLuint texture) {
    Logcat::i("HWVC", "Render::renderFilter %d", texture);
    filter->draw(texture);
#if 1
    if (yuvReadFilter) {
        yuvReadFilter->draw(filter->getFrameBuffer()->getFrameTexture());
    }
    //Test fbo read.
    ++count;
    if (count >= 150) {
        count = 0;
        int64_t time = TimeUtils::getCurrentTimeUS();
        yuvReadFilter->getFrameBuffer()->read(pixels);
        FILE *file = fopen("/sdcard/pixels.yv12", "wb");
        size_t size = yuvReadFilter->getFrameBuffer()->width()
                      * yuvReadFilter->getFrameBuffer()->height() * 4;
        Logcat::i("HWVC", "HwAndroidFrameBuffer::read cost %lld, %dx%d",
                  TimeUtils::getCurrentTimeUS() - time,
                  yuvReadFilter->getFrameBuffer()->width(),
                  yuvReadFilter->getFrameBuffer()->height());
        fwrite(pixels, 1, size, file);
        fclose(file);
    }
#endif
}

void HwRender::notifyPixelsReady() {
    postEventAtFront(new Message(EVENT_COMMON_PIXELS_READY, nullptr));
}