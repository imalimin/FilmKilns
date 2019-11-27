/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageProcessor.h"
#include "AlImage.h"
#include "HwRender.h"
#include "HwScreen.h"
#include "ObjectBox.h"

AlImageProcessor::AlImageProcessor() : HwAbsProcessor("AlImageProcessor") {
    registerAnUnit(new AlImage(ALIAS_OF_IMAGE));
    registerAnUnit(new HwRender(ALIAS_OF_RENDER));
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    putObject("canvas", &mCanvasModel).to({ALIAS_OF_IMAGE});
    putObject("layers", ObjectBox::box(&mLayers)).to({ALIAS_OF_IMAGE});
    Message *msg = new Message(EVENT_COMMON_PREPARE, nullptr);
    postEvent(msg);
}

AlImageProcessor::~AlImageProcessor() {

}

void AlImageProcessor::onDestroy() {
    HwAbsProcessor::onDestroy();
}

void AlImageProcessor::prepare(HwWindow *win) {
    Message *msg = new Message(EVENT_COMMON_PREPARE, nullptr);
    msg->obj = new ObjectBox(new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlImageProcessor::updateWindow(HwWindow *win) {
    Message *msg = new Message(EVENT_SCREEN_UPDATE_WINDOW, nullptr);
    msg->obj = ObjectBox::box(new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlImageProcessor::setCanvas(int32_t w, int32_t h, int32_t color) {
    mCanvasModel.set(w, h, color);
    _notifyCanvasUpdate();
}

int32_t AlImageProcessor::addLayer(const char *path) {
    std::string str(path);
    auto *layer = AlImageLayerModel::create(&mLayerIdCreator, str);
    if (nullptr == layer) {
        return Hw::FAILED.code;
    }
    mLayers.push_front(layer);
    _notifyLayerUpdate();
    return layer->getId();
}

void AlImageProcessor::invalidate() {
    postEvent(new Message(EVENT_COMMON_INVALIDATE, nullptr));
}

void AlImageProcessor::_notifyCanvasUpdate() {
    putObject("canvas", &mCanvasModel).to({ALIAS_OF_IMAGE});
    postEvent(new Message(EVENT_AIMAGE_UPDATE_CANVAS, nullptr));
}

void AlImageProcessor::_notifyLayerUpdate() {
    putObject("layers", ObjectBox::box(&mLayers)).to({ALIAS_OF_IMAGE});
    postEvent(new Message(EVENT_AIMAGE_NEW_LAYER, nullptr));
}

int32_t AlImageProcessor::removeLayer(int32_t id) {
    return Hw::FAILED.code;
}

int32_t AlImageProcessor::setScale(int32_t id, float scale) {
    return Hw::SUCCESS.code;
}

int32_t AlImageProcessor::setRotation(int32_t id, float rotation) {
    for (AlImageLayerModel *it:mLayers) {
        if (id == it->getId()) {
            it->setRotation(rotation);
            return Hw::SUCCESS.code;
        }
    }
    return Hw::SUCCESS.code;
}

int32_t AlImageProcessor::setTranslate(int32_t id, float x, float y) {
    return Hw::SUCCESS.code;
}

int32_t AlImageProcessor::setAlpha(int32_t id, float alpha) {
    return Hw::SUCCESS.code;
}
