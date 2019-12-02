/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayer.h"
#include "HwTexture.h"
#include "AlVec4.h"
#include "Logcat.h"

#define TAG "AlImageLayer"

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
    AlSize canvasSize(canvas->getWidth(), canvas->getHeight());
    //Set render params
    _measure(canvasSize);
    //Draw layer
    glViewport(0, 0, canvas->getWidth(), canvas->getHeight());
    mCanvasDrawer->draw(this->tex, canvas->getOutput());
}

void AlImageLayer::_measure(AlSize &canvasSize) {
    AlSize src(this->tex->getWidth(), this->tex->getHeight());
    /// 对图层和画布进行正交投影计算，转换坐标系，保证图像旋转缩放不会变形，并得到归一化的区域
    measure.update(src, canvasSize);
    measure.setScale(model->getScale().x, model->getScale().y);
    measure.setRotation(model->getRotation());
    ///TODO 矩阵Y轴与正常坐标系Y轴相反
    measure.setTranslate(model->getPosition().x, -model->getPosition().y);
    AlMatrix mat = measure.getMatrix();
    float vertex[8];
    measure.getVertex(vertex);
    AlVec2 lt;
    AlVec2 lb;
    AlVec2 rb;
    AlVec2 rt;
    ///获得经过位移旋转缩放变换后图像的位置坐标
    measure.getTransLORectF(lt, lb, rb, rt);
    ///设置Drawer的变换矩阵
    mCanvasDrawer->setAlpha(model->getAlpha());
    mCanvasDrawer->setMatrix(mat);
    ///设置纹理顶点
    mCanvasDrawer->setVertex(vertex);
    model->setQuad(lt, lb, rb, rt);
    ///TODO 这里需要把Y轴翻转一次
    model->getQuad().mirrorVertical();
}