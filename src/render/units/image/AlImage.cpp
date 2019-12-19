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

bool AlImage::onCreate(AlMessage *msg) {
    texAllocator = new TextureAllocator();
    return true;
}

bool AlImage::onDestroy(AlMessage *msg) {
    mLayerManager.release();
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    return true;
}

bool AlImage::onUpdateLayer(AlMessage *msg) {
    mLayerManager.update(getLayers(), texAllocator);
    return true;
}

bool AlImage::onInvalidate(AlMessage *m) {
    _notifyAll(m->arg1);
    return true;
}

std::vector<AlImageLayerModel *> *AlImage::getLayers() {
    auto *obj = static_cast<ObjectBox *>(getObject("layers"));
    return static_cast<vector<AlImageLayerModel *> *>(obj->ptr);
}

void AlImage::_notifyAll(int32_t flag) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_RENDER_CLEAR);
    msg->arg1 = (0 != (flag & 0x2));
    msg->desc = "clear";
    postEvent(msg);
    if (!mLayerManager.empty()) {
        int size = mLayerManager.size();
        for (int i = 0; i < size; ++i) {
            _notifyDescriptor(mLayerManager.getLayer(i));
        }
    }
    if (0 == (flag & 0x1)) {
        AlMessage *sMsg = AlMessage::obtain(EVENT_LAYER_RENDER_SHOW);
        sMsg->desc = "show";
        postEvent(sMsg);
    }
}

void AlImage::_notifyDescriptor(AlImageLayer *layer) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MEASURE, ObjectBox::wrap(layer));
    msg->desc = "measure";
    postEvent(msg);
}
