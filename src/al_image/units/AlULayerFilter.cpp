/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlULayerFilter.h"
#include "AlPaintFilter.h"
#include "HwNormalFilter.h"
#include "AlAbsMFilterAction.h"
#include "AlMPaintAction.h"
#include "AlLayerPair.h"
#include "AlTexManager.h"

#define TAG "AlULayerFilter"

AlULayerFilter::AlULayerFilter(string alias) : Unit(alias) {
    registerEvent(EVENT_LAYER_FILTER_RENDER,
                  reinterpret_cast<EventFunc>(&AlULayerFilter::onDoFilterAction));
    registerEvent(EVENT_LAYER_REMOVE_CACHE_LAYER,
                  reinterpret_cast<EventFunc>(&AlULayerFilter::onRemoveLayer));
}

AlULayerFilter::~AlULayerFilter() {

}

bool AlULayerFilter::onCreate(AlMessage *msg) {
    copyFilter = new HwNormalFilter();
    copyFilter->prepare();
    paintFilter = new AlPaintFilter();
    paintFilter->prepare();
//    roundFilter = new AlPaintRoundFilter();
//    roundFilter->prepare();
    return true;
}

bool AlULayerFilter::onDestroy(AlMessage *msg) {
    delete roundFilter;
    roundFilter = nullptr;
    delete copyFilter;
    copyFilter = nullptr;
    delete paintFilter;
    paintFilter = nullptr;
    return true;
}

bool AlULayerFilter::onDoFilterAction(AlMessage *msg) {
    Logcat::i(TAG, "%s(%d) layer size: %d", __FUNCTION__, __LINE__, layers.size());
    AlLayerPair *pair = msg->getObj<AlLayerPair *>();
    if (pair->model->countFilterAction() <= 0) {
        _notifyDescriptor(pair->layer, pair->model, msg->arg1);
        return true;
    }
    auto *fLayer = _findLayer(pair->model, pair->layer);
    if (nullptr == fLayer) {
        _notifyDescriptor(pair->layer, pair->model, msg->arg1);
        return true;
    }
    auto *actions = pair->model->getAllActions();
    auto itr = actions->begin();
    while (actions->end() != itr) {
        AlAbsMFilterAction *action = dynamic_cast<AlAbsMFilterAction *>(*itr);
        if (typeid(AlMPaintAction) == typeid(*action)) {
            if (pair->layer->getWidth() != fLayer->getWidth()
                || pair->layer->getHeight() != fLayer->getHeight()) {
                fLayer->getTexture()->update(nullptr,
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
            glViewport(0, 0, fLayer->getWidth(), fLayer->getHeight());
            paintFilter->draw(fLayer->getTexture(), fLayer->getTexture());
        }
        ++itr;
    }
    _notifyDescriptor(fLayer, pair->model, msg->arg1);
    return true;
}

bool AlULayerFilter::onRemoveLayer(AlMessage *msg) {
    auto itr = layers.find(msg->arg1);
    if (layers.end() != itr) {
        auto *layer = itr->second;
        auto *tex = layer->getTexture();
        AlTexManager::instance()->recycle(&tex);
        delete layer;
        layers.erase(itr);
    }
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

AlImageLayer *AlULayerFilter::_findLayer(AlImageLayerModel *model, AlImageLayer *layer) {
    if (nullptr == model) {
        return nullptr;
    }
    int32_t id = model->getId();
    auto itr = layers.find(id);
    if (layers.end() == itr) {
        auto *l = AlImageLayer::create(AlTexManager::instance()->alloc());
        if (l) {
            layers.insert(std::pair<int32_t, AlImageLayer *>(id, l));
            l->getTexture()->update(nullptr, layer->getWidth(), layer->getHeight(), GL_RGBA);
            glViewport(0, 0, layer->getWidth(), layer->getHeight());
            copyFilter->draw(layer->getTexture(), l->getTexture());
        }
        return l;
    }
    return itr->second;
}
