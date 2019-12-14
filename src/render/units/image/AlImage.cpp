/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImage.h"
#include "HwTexture.h"
#include "ObjectBox.h"

AlImage::AlImage(string alias) : Unit(alias) {
    registerEvent(EVENT_COMMON_INVALIDATE, reinterpret_cast<EventFunc>(&AlImage::onInvalidate));
    registerEvent(EVENT_AIMAGE_UPDATE_LAYER, reinterpret_cast<EventFunc>(&AlImage::onUpdateLayer));
}

AlImage::~AlImage() {
}

bool AlImage::onCreate(Message *msg) {
    texAllocator = new TextureAllocator();
    return true;
}

bool AlImage::onDestroy(Message *msg) {
    mLayerManager.release();
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    return true;
}

bool AlImage::onUpdateLayer(Message *msg) {
    mLayerManager.update(getLayers(), texAllocator);
    _notifyAll();
    return true;
}

bool AlImage::onInvalidate(Message *m) {
    _notifyAll();
    return true;
}

std::vector<AlImageLayerModel *> *AlImage::getLayers() {
    auto *obj = static_cast<ObjectBox *>(getObject("layers"));
    return static_cast<vector<AlImageLayerModel *> *>(obj->ptr);
}

void AlImage::_notifyAll() {
    if (!mLayerManager.empty()) {
        postEvent(new Message(EVENT_LAYER_RENDER_CLEAR));
        int size = mLayerManager.size();
        for (int i = 0; i < size; ++i) {
            _notifyDescriptor(mLayerManager.getLayer(i));
        }
        postEvent(new Message(EVENT_LAYER_RENDER_SHOW, nullptr, Message::QUEUE_MODE_UNIQUE));
    }
}

void AlImage::_notifyDescriptor(AlImageLayer *layer) {
    Message *msg = new Message(EVENT_LAYER_MEASURE, ObjectBox::wrap(layer), Message::QUEUE_MODE_FIRST_ALWAYS);
    postEvent(msg);
}

AlGLContext *AlImage::_getContext() {
    return dynamic_cast<AlGLContext *>(getObject("AL_CONTEXT"));
}
