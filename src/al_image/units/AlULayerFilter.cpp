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
#include "HwNormalFilter.h"
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
    mCopyFilter = new HwNormalFilter();
    return true;
}

bool AlULayerFilter::onDestroy(AlMessage *msg) {
    delete mCopyFilter;
    delete mosaicFilter;
    delete texAllocator;
    mCopyFilter = nullptr;
    mosaicFilter = nullptr;
    texAllocator = nullptr;
    return true;
}

bool AlULayerFilter::onDoFilterAction(AlMessage *msg) {
    Logcat::i(TAG, "%s(%d) onDoFilterAction", __FUNCTION__, __LINE__);
    AlImageLayer *layer = msg->getObj<ObjectBox *>()->unWrap<AlImageLayer *>();
    auto *actions = layer->model->getAllActions();
    auto itr = actions->begin();
//    while (actions->end() != itr) {
//        AlAbsMFilterAction *action = dynamic_cast<AlAbsMFilterAction *>(*itr);
//        if (typeid(AlMMosaicAction) == typeid(*action)) {
//            if (nullptr == mFilterTex) {
//                mosaicFilter->prepare();
//                mCopyFilter->prepare();
//                AlTexDescription desc;
//                desc.size.width = layer->getWidth();
//                desc.size.height = layer->getHeight();
//                mFilterTex = texAllocator->alloc(desc);
//            } else if (layer->getWidth() != mFilterTex->getWidth()
//                       || layer->getHeight() != mFilterTex->getHeight()) {
//                mFilterTex->update(nullptr, layer->getWidth(), layer->getHeight(), GL_RGBA);
//            }
//            dynamic_cast<AlMosaicFilter *>(mosaicFilter)->updatePath(
//                    dynamic_cast<AlMMosaicAction *>(action)->getPath());
//            mCopyFilter->draw(layer->getTexture(), mFilterTex);
//            mosaicFilter->draw(mFilterTex, layer->getTexture());
//        }
//        ++itr;
//    }
    _notifyDescriptor(layer);
    return true;
}

void AlULayerFilter::_notifyDescriptor(AlImageLayer *layer) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MEASURE, ObjectBox::wrap(layer));
    msg->desc = "measure";
    postEvent(msg);
}
