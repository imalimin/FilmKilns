/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlULayerFilter.h"
#include "AlPaintFilter.h"
#include "AlAbsMFilterAction.h"
#include "AlMPaintAction.h"
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
    paintFilter = new AlPaintFilter();
    paintFilter->prepare();
    nLayer = AlImageLayer::create(texAllocator->alloc());
    return true;
}

bool AlULayerFilter::onDestroy(AlMessage *msg) {
    delete nLayer;
    delete paintFilter;
    delete texAllocator;
    nLayer = nullptr;
    paintFilter = nullptr;
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
        if (typeid(AlMPaintAction) == typeid(*action)) {
            if (pair->layer->getWidth() != nLayer->getWidth()
                || pair->layer->getHeight() != nLayer->getHeight()) {
                nLayer->getTexture()->update(nullptr,
                                             pair->layer->getWidth(),
                                             pair->layer->getHeight(),
                                             GL_RGBA);
            }
            AlMPaintAction *nAction = dynamic_cast<AlMPaintAction *>(action);
            dynamic_cast<AlPaintFilter *>(paintFilter)->setColor(nAction->getColor());
            dynamic_cast<AlPaintFilter *>(paintFilter)->setPaintSize(nAction->getPaintSize());
            std::vector<float> *path = new std::vector<float>;
            nAction->getDiffPath(*path);
            dynamic_cast<AlPaintFilter *>(paintFilter)->setPath(path, true);
            path->clear();
            delete path;
            glViewport(0, 0, nLayer->getWidth(), nLayer->getHeight());
            paintFilter->draw(pair->layer->getTexture(), pair->layer->getTexture());
        }
        ++itr;
    }
    _notifyDescriptor(pair->layer, pair->model, msg->arg1);
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
