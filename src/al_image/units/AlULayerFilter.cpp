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
#include "AlLayerPair.h"

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
    nLayer = AlImageLayer::create(texAllocator->alloc());
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
    AlLayerPair *pair = msg->getObj<AlLayerPair *>();
    if (pair->model->countFilterAction() <= 0) {
        _notifyDescriptor(pair->layer, pair->model, msg->arg1);
        return true;
    }
    auto *actions = pair->model->getAllActions();
    auto itr = actions->begin();
    while (actions->end() != itr) {
        AlAbsMFilterAction *action = dynamic_cast<AlAbsMFilterAction *>(*itr);
        if (typeid(AlMMosaicAction) == typeid(*action)) {
            if (pair->layer->getWidth() != nLayer->getWidth()
                || pair->layer->getHeight() != nLayer->getHeight()) {
                nLayer->getTexture()->update(nullptr,
                                             pair->layer->getWidth(),
                                             pair->layer->getHeight(),
                                             GL_RGBA);
            }
            dynamic_cast<AlMosaicFilter *>(mosaicFilter)->updatePath(
                    dynamic_cast<AlMMosaicAction *>(action)->getPath());
            mosaicFilter->draw(pair->layer->getTexture(), nLayer->getTexture());
        }
        ++itr;
    }
    _notifyDescriptor(nLayer, pair->model, msg->arg1);
    return true;
}

void AlULayerFilter::_notifyDescriptor(AlImageLayer *layer,
                                       AlImageLayerModel *model,
                                       int32_t flags) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MEASURE, new AlLayerPair(layer, model));
    msg->arg1 = flags;
    msg->desc = "measure";
    postEvent(msg);
}
