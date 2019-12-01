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
    AlSize sSize(this->tex->getWidth(), this->tex->getHeight());
    AlSize dSize(canvas->getWidth(), canvas->getHeight());
    _calculateLayerQuad(sSize, dSize);
    //Draw layer
    glViewport(0, 0, canvas->getWidth(), canvas->getHeight());
    mCanvasDrawer->draw(this->tex, canvas->getOutput());
}

void AlImageLayer::_applyParams() {
    mCanvasDrawer->setAlpha(model->getAlpha());
    mCanvasDrawer->setScale(model->getScale().x, model->getScale().y);
    mCanvasDrawer->setRotation(model->getRotation());
    ///矩阵Y轴与正常坐标系Y轴相反
    mCanvasDrawer->setTranslate(model->getPosition().x, -model->getPosition().y);
}

void AlImageLayer::_calculateLayerQuad(AlSize &src, AlSize &dest) {
    if (nullptr == model) {
        return;
    }
    float aspectRatio = dest.width > dest.height ?
                        (float) dest.width / (float) dest.height :
                        (float) dest.height / (float) dest.width;
    float width = 0.0f, height = 0.0f;
    if (dest.width > dest.height) {
        width = src.width / (float) src.height / aspectRatio * model->getScale().x;
        height = 1.0f * model->getScale().y;
    } else {
        width = 1.0f * model->getScale().x;
        height = src.height / (float) src.width / aspectRatio * model->getScale().y;
    }
    AlPointF leftTop(-width + model->getPosition().x, height + model->getPosition().y);
    AlPointF leftBottom(-width + model->getPosition().x, -height + model->getPosition().y);
    AlPointF rightBottom(width + model->getPosition().x, -height + model->getPosition().y);
    AlPointF rightTop(width + model->getPosition().x, height + model->getPosition().y);
    model->setQuad(leftTop, leftBottom, rightBottom, rightTop);
}
