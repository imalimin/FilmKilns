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
    AlSize dSize(canvas->getWidth(), canvas->getHeight());
    //Set render params
    _applyParams(dSize);
    //Draw layer
    glViewport(0, 0, canvas->getWidth(), canvas->getHeight());
    mCanvasDrawer->draw(this->tex, canvas->getOutput());
}

void AlImageLayer::_applyParams(AlSize &dest) {
    AlSize sSize(this->tex->getWidth(), this->tex->getHeight());
    AlRectF sRectF, dRectF;
    _calculateRect(sSize, dest, sRectF, dRectF);
    oMat.update(dRectF.left, dRectF.right, dRectF.bottom, dRectF.top, -1.0f, 1.0f);

    _setScale(model->getScale().x, model->getScale().y);
    _setRotation(model->getRotation());
    ///TODO 矩阵Y轴与正常坐标系Y轴相反
    _setTranslate(model->getPosition().x, -model->getPosition().y, dRectF);

    ///设置Drawer的变换矩阵
    mCanvasDrawer->setAlpha(model->getAlpha());
    AlMatrix mat = oMat * tMat;
    mCanvasDrawer->setMatrix(mat);
    float *vertex = new float[8]{
            sRectF.left, sRectF.bottom, //LEFT,BOTTOM
            sRectF.right, sRectF.bottom, //RIGHT,BOTTOM
            sRectF.left, sRectF.top, //LEFT,TOP
            sRectF.right, sRectF.top,//RIGHT,TOP
    };
    /// 根据Window大小设置纹理顶点
    /// 保证图片大边总是填充满Canvas小边
    /// 此时layer model的scale为1
    mCanvasDrawer->setVertex(vertex);
    delete[] vertex;
    /// 计算图片旋转缩放位移后所在的区域
    /// 与Shader中的uTextureMatrix * aPosition一致
    AlVec4 leftTop(sRectF.left, sRectF.top), leftBottom(sRectF.left, sRectF.bottom);
    AlVec4 rightBottom(sRectF.right, sRectF.bottom), rightTop(sRectF.right, sRectF.top);
    auto lt = leftTop.multiply(mat).xy();
    auto lb = leftBottom.multiply(mat).xy();
    auto rb = rightBottom.multiply(mat).xy();
    auto rt = rightTop.multiply(mat).xy();
    model->setQuad(lt, lb, rb, rt);
    ///TODO 这里需要把Y轴翻转一次
    model->getQuad().mirrorVertical();
}

void AlImageLayer::_setScale(float scaleX, float scaleY) {
    tMat.setScale(scaleX, scaleY);
}

void AlImageLayer::_setRotation(float rotation) {
    tMat.setRotation(rotation);
}

void AlImageLayer::_setTranslate(float x, float y, AlRectF rectF) {
    tMat.setTranslate(x * rectF.getWidth() / 2.0f, y * rectF.getHeight() / 2.0f);
}

void AlImageLayer::_calculateRect(AlSize &src, AlSize &dest,
                                  AlRectF &srcRectF, AlRectF &destRectF) {
    float aspectRatio = dest.width > dest.height ?
                        (float) dest.width / (float) dest.height :
                        (float) dest.height / (float) dest.width;

    //计算正交矩阵
    if (dest.width > dest.height) {
        destRectF.left = -aspectRatio;
        destRectF.right = -destRectF.left;
        destRectF.bottom = -1.0f;
        destRectF.top = -destRectF.bottom;
    } else {
        destRectF.left = -1.0f;
        destRectF.right = -destRectF.left;
        destRectF.bottom = -aspectRatio;
        destRectF.top = -destRectF.bottom;
    }
    /// 根据Canvas大小计算纹理顶点
    /// 保证图片总是完整填充到Canvas
    /// 并保证至Layer和Canvas至少有一边相等
    /// 此时layer model的scale=1为默认状态
    if (src.width / (float) src.height > dest.width / (float) dest.height) {
        srcRectF.left = destRectF.left;
        srcRectF.right = -srcRectF.left;
        srcRectF.bottom = srcRectF.left * src.height / (float) src.width;
        srcRectF.top = -srcRectF.bottom;
    } else {
        srcRectF.bottom = destRectF.bottom;
        srcRectF.top = -srcRectF.bottom;
        srcRectF.left = srcRectF.bottom * src.width / (float) src.height;
        srcRectF.right = -srcRectF.left;
    }
}