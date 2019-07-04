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

HwRender::HwRender() : HwRender(nullptr) {
}

HwRender::HwRender(HandlerThread *handlerThread) : Unit(handlerThread) {
    name = __FUNCTION__;
#ifdef ANDROID
    filter = new NormalFilter(true);
#else
    filter = new NormalFilter();
#endif
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwRender::eventPrepare));
    registerEvent(EVENT_RENDER_FILTER, reinterpret_cast<EventFunc>(&HwRender::eventFilter));
    registerEvent(EVENT_RENDER_SET_FILTER, reinterpret_cast<EventFunc>(&HwRender::eventSetFilter));
}

HwRender::~HwRender() {
}

bool HwRender::eventRelease(Message *msg) {
    Logcat::i("HWVC", "Render::eventRelease");
    post([this] {
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
    return true;
}

void HwRender::checkFilter(int width, int height) {
    if (filter) {
        bool ret = filter->init(width, height);
    }
    if (!pixels) {
        pixels = new uint8_t[width * height * 4];
    }
}

void HwRender::renderFilter(GLuint texture) {
    Logcat::i("HWVC", "Render::renderFilter %d", texture);
    filter->draw(texture);
#if 0
    //Test fbo read.
    ++count;
    if (count >= 150) {
        count = 0;
        int64_t time = TimeUtils::getCurrentTimeUS();
        filter->getFrameBuffer()->read(pixels);
        FILE *file = fopen("/sdcard/pixels.bmp", "wb");
        size_t size = filter->getFrameBuffer()->width()
                      * filter->getFrameBuffer()->height() * 4;
        Logcat::i("HWVC", "HwAndroidFrameBuffer::read cost %lld, %dx%d",
                  TimeUtils::getCurrentTimeUS() - time,
                  filter->getFrameBuffer()->width(),
                  filter->getFrameBuffer()->height());
        fwrite(pixels, 1, size, file);
        fclose(file);
    }
#endif
    if (!buf) {
        size_t size = static_cast<size_t>(filter->getFrameBuffer()->width()
                                          * filter->getFrameBuffer()->height() * 4);
        buf = HwBuffer::alloc(size);
    }
    if (filter->getFrameBuffer()->read(buf->getData())) {
        Message *msg = new Message(EVENT_VIDEO_OUT_WRITE, nullptr);
        msg->obj = buf;
        postEvent(msg);
    }
}

void HwRender::renderScreen() {
    Logcat::i("HWVC", "Render::renderScreen");
    Message *msg = new Message(EVENT_SCREEN_DRAW, nullptr);
    msg->obj = new ObjectBox(new Size(filter->getFrameBuffer()->width(),
                                      filter->getFrameBuffer()->height()));
    msg->arg1 = filter->getFrameBuffer()->getFrameTexture();
    postEvent(msg);
}

bool HwRender::eventPrepare(Message *msg) {
    Logcat::i("HWVC", "Render::eventPrepare");
    return true;
}

bool HwRender::eventFilter(Message *msg) {
    Logcat::i("HWVC", "Render::eventFilter");
    Size *size = static_cast<Size *>(msg->tyrUnBox());
    GLuint tex = msg->arg1;
    post([this, size, tex] {
        checkFilter(size->width, size->height);
        glViewport(0, 0, size->width, size->height);
        renderFilter(tex);
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