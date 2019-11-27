/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayer.h"
#include "HwTexture.h"

AlImageLayer *AlImageLayer::create(AlImageLayerModel *model, HwAbsTexture *tex) {
    return new AlImageLayer(model, tex);
}

AlImageLayer::AlImageLayer(AlImageLayerModel *model, HwAbsTexture *tex) : Object() {
    this->model = model;
    this->tex = tex;
}

AlImageLayer::~AlImageLayer() {
    delete mCanvasDrawer;
    mCanvasDrawer = nullptr;
    model = nullptr;
}

void AlImageLayer::draw(AlImageCanvas *canvas) {
    if (nullptr == mCanvasDrawer) {
        mCanvasDrawer = new AlCanvasDrawer();
        mCanvasDrawer->prepare();
    }
    HwAbsTexture *canvasTex = canvas->getOutput();
    if (canvasTex) {
        _draw(canvas);
        delete canvasTex;
    }
}

void AlImageLayer::_draw(AlImageCanvas *canvas) {
    float canvasRatio = canvas->getWidth() / (float) canvas->getHeight();
    float layerRatio = getWidth() / (float) getHeight();
    float scaleX = 1.0f, scaleY = 1.0f;
    if (canvasRatio > layerRatio) {
        scaleX = layerRatio / canvasRatio;
        scaleY = 1.0f;
    } else {
        scaleX = 1.0f;
        scaleY = canvasRatio / layerRatio;
    }
    mCanvasDrawer->setScale(scaleX, scaleY);
    mCanvasDrawer->setRotation(model->getRotation());
    //Draw
    glViewport(0, 0, canvas->getWidth(), canvas->getHeight());
    mCanvasDrawer->draw(this->tex, canvas->getOutput());
}

int32_t AlImageLayer::getWidth() {
    return this->tex->getWidth();
}

int32_t AlImageLayer::getHeight() {
    return this->tex->getHeight();
}
