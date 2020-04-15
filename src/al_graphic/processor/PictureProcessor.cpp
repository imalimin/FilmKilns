//
// Created by mingyi.li on 2018/12/25.
//

#include "../include/PictureProcessor.h"
#include "../include/HwRender.h"
#include "../include/HwScreen.h"
#include "../include/Image.h"
#include "../include/NativeWindow.h"
#include "ObjectBox.h"

PictureProcessor::PictureProcessor() {
    pipeline = new UnitPipeline(__FUNCTION__);
    pipeline->registerAnUnit(new Image(ALIAS_OF_IMAGE));
    pipeline->registerAnUnit(new HwRender(ALIAS_OF_RENDER));
    //注意顺序问题，包含EGL环境的模块放到最后，因为要最后释放
    pipeline->registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
}

PictureProcessor::~PictureProcessor() {
    if (pipeline) {
        pipeline->postDestroy();
        delete pipeline;
        pipeline = nullptr;
    }
}

void PictureProcessor::prepare(HwWindow *win) {
    if (pipeline) {
        AlMessage *msg = AlMessage::obtain(EVENT_COMMON_PREPARE, new NativeWindow(win, nullptr));
        pipeline->postEvent(msg);
    }
}

void PictureProcessor::updateWindow(HwWindow *win) {
    if (pipeline) {
        AlMessage *msg = AlMessage::obtain(EVENT_SCREEN_UPDATE_WINDOW,
                                           new NativeWindow(win, nullptr));
        pipeline->postEvent(msg);
    }
}

void PictureProcessor::show(const string file) {
    if (!pipeline) return;
    AlMessage *msg = AlMessage::obtain(EVENT_IMAGE_SHOW);
    msg->obj = new ObjectBox(new string(file));
    pipeline->postEvent(msg);
}


void PictureProcessor::setFilter(HwAbsFilter *filter) {
    AlMessage *msg = AlMessage::obtain(EVENT_RENDER_SET_FILTER);
    msg->obj = ObjectBox::wrap(filter);
    pipeline->postEvent(msg);
}

void PictureProcessor::invalidate() {
    if (!pipeline) return;
    AlMessage *invalidateMsg = AlMessage::obtain(EVENT_COMMON_INVALIDATE);
    pipeline->postEvent(invalidateMsg);
}