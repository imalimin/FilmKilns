/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "Logcat.h"
#include "../include/HwRender.h"
#include "../include/HwNormalFilter.h"
#include "../include/ObjectBox.h"
#include "TimeUtils.h"
#include "../include/HwRGBA2NV12Filter.h"
#include "../include/HwAbsTexture.h"
#include "../include/HwTexture.h"
#include "../include/HwFBObject.h"
#include <GLES2/gl2.h>

#ifdef ANDROID

#include "../platform/android/entity/HwAndroidTexture.h"

#endif

HwRender::HwRender(string alias) : Unit(alias) {
    filter = new HwNormalFilter();
    yuvReadFilter = new HwRGBA2NV12Filter();
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
    if (yuvReadFilter) {
        delete yuvReadFilter;
        yuvReadFilter = nullptr;
    }
    if (filter) {
        delete filter;
        Logcat::i("HWVC", "Render::eventRelease filter");
        filter = nullptr;
    }
    if (buf) {
        delete buf;
        buf = nullptr;
    }
    if (yuvTarget) {
        delete yuvTarget;
        yuvTarget = nullptr;
    }
    if (target) {
        delete target;
        target = nullptr;
    }
    if (fbo) {
        delete fbo;
        fbo = nullptr;
    }
    return true;
}

bool HwRender::eventReadPixels(Message *msg) {
    bool read = false;
    if (yuvReadFilter) {
        glViewport(0, 0, yuvTarget->getWidth(), yuvTarget->getHeight());
        yuvReadFilter->draw(target, yuvTarget);
        if (fbo->read(buf->data())) {
            read = true;
        }
    }
    if (!read && fbo->read(buf->data())) {
        read = true;
    }
    if (read) {
        Message *msg1 = new Message(EVENT_COMMON_PIXELS, nullptr);
        msg1->obj = HwBuffer::wrap(buf->data(), buf->size());
        msg1->arg2 = tsInNs;
        postEvent(msg1);
    }
    return true;
}

bool HwRender::eventRenderFilter(Message *msg) {
    Logcat::i("HWVC", "Render::eventFilter");
    HwAbsTexture *tex = static_cast<HwAbsTexture *>(msg->obj);
    tsInNs = msg->arg2;
    checkEnv(tex->getWidth(), tex->getHeight());
    glViewport(0, 0, tex->getWidth(), tex->getHeight());
    renderFilter(tex);
    notifyPixelsReady();
    renderScreen();
    return true;
}

bool HwRender::eventSetFilter(Message *msg) {
    Logcat::i("HWVC", "Render::eventSetFilter");
    HwAbsFilter *newFilter = static_cast<HwAbsFilter *>(msg->tyrUnBox());
    if (filter) {
        delete filter;
        filter = nullptr;
    }
    filter = newFilter;
    return true;
}

void HwRender::renderScreen() {
    Logcat::i("HWVC", "Render::renderScreen");
    Message *msg = new Message(EVENT_SCREEN_DRAW, nullptr);
    msg->obj = new ObjectBox(new Size(target->getWidth(), target->getHeight()));
    msg->arg1 = target->texId();
    postEvent(msg);
}

void HwRender::checkEnv(int width, int height) {
    if (filter) {
        bool ret = filter->prepare();
        if (ret) {
            target = HwTexture::alloc(GL_TEXTURE_2D);
            target->update(nullptr, width, height);
        }
    }
    if (yuvReadFilter) {
        bool ret = yuvReadFilter->prepare();
        if (ret) {
            yuvTarget = HwTexture::alloc(GL_TEXTURE_2D);
            yuvTarget->update(nullptr, width / 4, height * 3 / 2);
            fbo = HwFBObject::alloc();
            fbo->bindTex(yuvTarget);
            size_t size = static_cast<size_t>(width * height * 3 / 2);
            buf = HwBuffer::alloc(size);
        }
    }
}

void HwRender::renderFilter(HwAbsTexture *tex) {
    Logcat::i("HWVC", "Render::renderFilter %d", tex->texId());
    filter->draw(tex, target);
}

void HwRender::notifyPixelsReady() {
    postEvent(new Message(EVENT_COMMON_PIXELS_READY, nullptr, Message::QUEUE_MODE_FIRST_ALWAYS,
                          nullptr));
}