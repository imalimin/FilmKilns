/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlULayerFilter.h"
#include "AlMosaicFilter.h"
#include "AlAbsMFilterAction.h"
#include "AlMMosaicAction.h"
#include "ObjectBox.h"

#define TAG "AlULayerFilter"

AlULayerFilter::AlULayerFilter(string alias) : Unit(alias) {
    registerEvent(EVENT_LAYER_FILTER_RENDER,
                  reinterpret_cast<EventFunc>(&AlULayerFilter::onDoFilterAction));
}

AlULayerFilter::~AlULayerFilter() {

}

bool AlULayerFilter::onCreate(AlMessage *msg) {
    texAllocator = new AlTexAllocator();
    mosaicFilter = new AlMosaicFilter();
    mosaicFilter->prepare();
    nLayer = AlImageLayer::create(nullptr, texAllocator->alloc());
    return true;
}

bool AlULayerFilter::onDestroy(AlMessage *msg) {
    delete nLayer;
    delete mosaicFilter;
    delete texAllocator;
    nLayer = nullptr;
    mosaicFilter = nullptr;
    texAllocator = nullptr;
    return true;
}

bool AlULayerFilter::onDoFilterAction(AlMessage *msg) {
    Logcat::i(TAG, "%s(%d) onDoFilterAction", __FUNCTION__, __LINE__);
    AlImageLayer *layer = msg->getObj<ObjectBox *>()->unWrap<AlImageLayer *>();
    if (layer->model->countFilterAction() <= 0) {
        _notifyDescriptor(layer);
        return true;
    }
    auto *actions = layer->model->getAllActions();
    auto itr = actions->begin();
    while (actions->end() != itr) {
        AlAbsMFilterAction *action = dynamic_cast<AlAbsMFilterAction *>(*itr);
        if (typeid(AlMMosaicAction) == typeid(*action)) {
            if (layer->getWidth() != nLayer->getWidth()
                || layer->getHeight() != nLayer->getHeight()) {
                nLayer->getTexture()->update(nullptr, layer->getWidth(), layer->getHeight(),
                                             GL_RGBA);
            }
            dynamic_cast<AlMosaicFilter *>(mosaicFilter)->updatePath(
                    dynamic_cast<AlMMosaicAction *>(action)->getPath());
            mosaicFilter->draw(layer->getTexture(), nLayer->getTexture());
        }
        ++itr;
    }
    nLayer->model = layer->model;
    _notifyDescriptor(nLayer);
    return true;
}

void AlULayerFilter::_notifyDescriptor(AlImageLayer *layer) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MEASURE, ObjectBox::wrap(layer));
    msg->desc = "measure";
    postEvent(msg);
}
