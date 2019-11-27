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

int32_t AlImageLayer::getWidth() {
    return this->tex->getWidth();
}

int32_t AlImageLayer::getHeight() {
    return this->tex->getHeight();
}

void AlImageLayer::_draw(AlImageCanvas *canvas) {
    //Set render params
    _applyParams();
    //Draw layer
    glViewport(0, 0, canvas->getWidth(), canvas->getHeight());
    mCanvasDrawer->draw(this->tex, canvas->getOutput());
}

void AlImageLayer::_applyParams() {
    mCanvasDrawer->setAlpha(model->getAlpha());
    mCanvasDrawer->setScale(model->getScale().x, model->getScale().y);
    mCanvasDrawer->setRotation(model->getRotation());
    mCanvasDrawer->setTranslate(model->getPosition().x, model->getPosition().y);
}
