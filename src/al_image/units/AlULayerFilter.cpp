/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlULayerFilter.h"
#include "AlMosaicFilter.h"
#include "ObjectBox.h"

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
    mFilterTex = texAllocator->alloc();
    return true;
}

bool AlULayerFilter::onDestroy(AlMessage *msg) {
    delete mosaicFilter;
    delete texAllocator;
    mosaicFilter = nullptr;
    texAllocator = nullptr;
    return true;
}

bool AlULayerFilter::onDoFilterAction(AlMessage *msg) {
//    if(description->mosaicPath) {
//        mFilterTex->update(nullptr, getWidth(), getHeight(), GL_RGBA);
//        dynamic_cast<AlMosaicFilter *>(mosaicFilter)->updatePath(description->mosaicPath);
//        mosaicFilter->draw(description->tex, mFilterTex);
//        mCanvasDrawer->draw(mFilterTex, mCanvasTex);
//    }
    return true;
}

void AlULayerFilter::_notifyDescriptor(AlImageLayer *layer) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MEASURE, ObjectBox::wrap(layer));
    msg->desc = "measure";
    postEvent(msg);
}
