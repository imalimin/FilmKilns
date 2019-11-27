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
    Message *msg = new Message(EVENT_COMMON_PREPARE);
    postEvent(msg);
}

AlImageProcessor::~AlImageProcessor() {

}

void AlImageProcessor::onDestroy() {
    HwAbsProcessor::onDestroy();
}

void AlImageProcessor::prepare(HwWindow *win) {
    Message *msg = new Message(EVENT_COMMON_PREPARE);
    msg->obj = new ObjectBox(new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlImageProcessor::updateWindow(HwWindow *win) {
    Message *msg = new Message(EVENT_SCREEN_UPDATE_WINDOW);
    msg->obj = ObjectBox::box(new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlImageProcessor::setCanvas(int32_t w, int32_t h, int32_t color) {
    mCanvasModel.set(w, h, color);
    _notifyCanvasUpdate();
}

void AlImageProcessor::invalidate() {
    postEvent(new Message(EVENT_COMMON_INVALIDATE, nullptr, Message::QUEUE_MODE_UNIQUE));
}

void AlImageProcessor::_notifyCanvasUpdate() {
    putObject("canvas", &mCanvasModel).to({ALIAS_OF_IMAGE});
    postEvent(new Message(EVENT_AIMAGE_UPDATE_CANVAS));
}

void AlImageProcessor::_notifyLayerUpdate() {
    putObject("layers", ObjectBox::box(&mLayers)).to({ALIAS_OF_IMAGE});
    postEvent(new Message(EVENT_AIMAGE_NEW_LAYER));
}

int32_t AlImageProcessor::addLayer(const char *path) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    std::string str(path);
    auto *layer = AlImageLayerModel::create(&mLayerIdCreator, str);
    if (nullptr == layer) {
        return Hw::FAILED.code;
    }
    mLayers.push_front(layer);
    _notifyLayerUpdate();
    return layer->getId();
}

int32_t AlImageProcessor::removeLayer(int32_t id) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    return Hw::FAILED.code;
}

int32_t AlImageProcessor::setScale(int32_t id, float scale) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer) {
        layer->setScale(scale, scale);
        invalidate();
        return Hw::SUCCESS.code;
    }
    return Hw::FAILED.code;
}

int32_t AlImageProcessor::setRotation(int32_t id, float rotation) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer) {
        layer->setRotation(rotation);
        invalidate();
        return Hw::SUCCESS.code;
    }
    return Hw::FAILED.code;
}

int32_t AlImageProcessor::setTranslate(int32_t id, float x, float y) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer) {
        layer->setPosition(x, y);
        invalidate();
        return Hw::SUCCESS.code;
    }
    return Hw::FAILED.code;
}

int32_t AlImageProcessor::setAlpha(int32_t id, float alpha) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer) {
        layer->setAlpha(alpha);
        invalidate();
        return Hw::SUCCESS.code;
    }
    return Hw::FAILED.code;
}

AlImageLayerModel *AlImageProcessor::_getLayer(int32_t id) {
    for (AlImageLayerModel *it : mLayers) {
        if (id == it->getId()) {
            return it;
        }
    }
    return nullptr;
}
