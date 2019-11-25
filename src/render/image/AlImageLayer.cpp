/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayer.h"
#include "HwTexture.h"

AlImageLayer *AlImageLayer::create(HwAbsTexture *tex) {
    return new AlImageLayer(tex);
}

AlImageLayer::AlImageLayer(HwAbsTexture *tex) : Object() {
    this->tex = tex;
}

AlImageLayer::~AlImageLayer() {
    delete mCanvasFilter;
    mCanvasFilter = nullptr;
}

void AlImageLayer::draw(AlImageCanvas *canvas) {
    if (nullptr == mCanvasFilter) {
        mCanvasFilter = new AlCanvasFilter();
        mCanvasFilter->prepare();
    }
    HwAbsTexture *canvasTex = canvas->getOutput();
    if (canvasTex) {
        float canvasRatio = canvas->getWidth() / (float) canvas->getHeight();
        float layerRatio = getWidth() / (float) getHeight();
        float scaleX = 1.0f, scaleY = 1.0f;
        if (canvasRatio > layerRatio) {
            scaleX =  layerRatio / canvasRatio;
            scaleY = 1.0f;
        } else {
            scaleX = 1.0f;
            scaleY = canvasRatio / layerRatio;
        }
        mCanvasFilter->setScale(scaleX, scaleY);
        //Draw
        glViewport(0, 0, canvas->getWidth(), canvas->getHeight());
        mCanvasFilter->draw(this->tex, canvas->getOutput());
        delete canvasTex;
    }
}

int32_t AlImageLayer::getWidth() {
    return this->tex->getWidth();
}

int32_t AlImageLayer::getHeight() {
    return this->tex->getHeight();
}
