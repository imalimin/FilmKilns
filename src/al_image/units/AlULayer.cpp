/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlULayer.h"
#include "HwTexture.h"
#include "ObjectBox.h"
#include "core/file/AlFileImporter.h"

#define TAG "AlULayer"

AlULayer::AlULayer(string alias) : Unit(alias) {
    registerEvent(EVENT_COMMON_INVALIDATE, reinterpret_cast<EventFunc>(&AlULayer::onInvalidate));
    registerEvent(EVENT_AIMAGE_UPDATE_LAYER, reinterpret_cast<EventFunc>(&AlULayer::onUpdateLayer));
    registerEvent(EVENT_AIMAGE_IMPORT, reinterpret_cast<EventFunc>(&AlULayer::onImport));
    registerEvent(EVENT_AIMAGE_REDO, reinterpret_cast<EventFunc>(&AlULayer::onRedo));
    registerEvent(EVENT_AIMAGE_UNDO, reinterpret_cast<EventFunc>(&AlULayer::onUndo));
}

AlULayer::~AlULayer() {
    this->onAlxLoadListener = nullptr;
}

bool AlULayer::onCreate(AlMessage *msg) {
    texAllocator = new AlTexAllocator();
    /// Just for init models address.
    mLayerManager.update(getLayers(), texAllocator);
    return true;
}

bool AlULayer::onDestroy(AlMessage *msg) {
    mLayerManager.release();
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    return true;
}

bool AlULayer::onUpdateLayer(AlMessage *msg) {
    mLayerManager.update(getLayers(), texAllocator);
    return true;
}

bool AlULayer::onInvalidate(AlMessage *m) {
    _notifyAll(m->arg1);
    return true;
}

std::vector<AlImageLayerModel *> *AlULayer::getLayers() {
    auto *obj = static_cast<ObjectBox *>(getObject("layers"));
    return static_cast<vector<AlImageLayerModel *> *>(obj->ptr);
}

void AlULayer::_notifyAll(int32_t flag) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_RENDER_CLEAR);
    msg->arg1 = (0 != (flag & 0x2));
    msg->desc = "clear";
    postEvent(msg);
    if (!mLayerManager.empty()) {
        int size = mLayerManager.size();
        for (int i = 0; i < size; ++i) {
            AlImageLayer *layer = mLayerManager.getLayer(i);
            if (layer->model->countFilterAction() > 0) {
                _notifyFilter(layer);
            } else {
                _notifyDescriptor(layer);
            }
        }
    }
    if (0 == (flag & 0x1)) {
        AlMessage *sMsg = AlMessage::obtain(EVENT_LAYER_RENDER_SHOW);
        sMsg->desc = "show";
        postEvent(sMsg);
    }
}

void AlULayer::_notifyDescriptor(AlImageLayer *layer) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MEASURE, ObjectBox::wrap(layer));
    msg->desc = "measure";
    postEvent(msg);
}

void AlULayer::_notifyFilter(AlImageLayer *layer) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_FILTER_RENDER, ObjectBox::wrap(layer));
    msg->desc = "filter";
    postEvent(msg);
}

bool AlULayer::onImport(AlMessage *m) {
    std::string path = m->desc;
    AlImageCanvasModel canvas;
    std::vector<AlImageLayerModel *> layers;
    AlFileImporter importer;
    if (Hw::SUCCESS != importer.importFromFile(path, &canvas, &layers)
        || layers.empty() || canvas.getWidth() <= 0 || canvas.getHeight() <= 0) {
        return true;
    }
    mLayerManager.replaceAll(texAllocator, &layers);
    layers.clear();
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_RENDER_UPDATE_CANVAS, nullptr,
                                       AlMessage::QUEUE_MODE_FIRST_ALWAYS);
    msg->obj = new AlSize(canvas.getWidth(), canvas.getHeight());
    postEvent(msg);
    _notifyAll();
    if (onAlxLoadListener) {
        onAlxLoadListener(mLayerManager.getMaxId());
    }
    return true;
}

bool AlULayer::onRedo(AlMessage *m) {
    return true;
}

bool AlULayer::onUndo(AlMessage *m) {
    return true;
}

void AlULayer::_saveStep() {

}

void AlULayer::setOnAlxLoadListener(AlULayer::OnAlxLoadListener listener) {
    this->onAlxLoadListener = listener;
}
