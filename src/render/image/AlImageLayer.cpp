/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayer.h"
#include "HwTexture.h"
#include "AlVec4.h"
#include "AlMath.h"
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
    model = nullptr;
}

int32_t AlImageLayer::getWidth() {
    return this->tex->getWidth();
}

int32_t AlImageLayer::getHeight() {
    return this->tex->getHeight();
}

HwResult AlImageLayer::measure(AlImageLayerDrawModel &drawModel) {
    if (nullptr == model) {
        return Hw::FAILED;
    }
    ///先把图层原始大小保存到AlImageLayerDrawModel，各种Operate中回用到
    drawModel.setLayerSize(this->tex->getWidth(), this->tex->getHeight());
    HwResult ret = _measureOperate(drawModel);
    AlSize canvasSize = drawModel.getCanvasSize();
    ///经过各种各样的Operate后，layer size会被改变并更新到AlImageLayerDrawModel
    ///这里需要获取最新的layer size，不然会出错
    ///必须裁剪Operate会改变layer size，如果不更新则可能出现图像拉伸
    AlSize src = drawModel.getLayerSize();
    /// 对图层和画布进行正交投影计算，转换坐标系，保证图像旋转缩放不会变形，并得到归一化的区域
    aMeasure.updateOrthogonal(src, canvasSize);
    aMeasure.setScale(model->getScale().x, model->getScale().y);
    aMeasure.setRotation(static_cast<float>(model->getRotation().toFloat() * AlMath::PI));
    ///TODO 矩阵Y轴与正常坐标系Y轴相反
    aMeasure.setTranslate(model->getPosition().x, -model->getPosition().y);
    aMeasure.measure(drawModel);
    drawModel.tex = HwTexture::wrap(dynamic_cast<HwTexture *>(tex));
    drawModel.alpha = model->getAlpha();
    ///Update quad.
    AlVec2 lt;
    AlVec2 lb;
    AlVec2 rb;
    AlVec2 rt;
    ///获得经过位移旋转缩放变换后图像的位置坐标
    aMeasure.getTransLORectF(lt, lb, rb, rt);
    model->setQuad(lt, lb, rb, rt);
    ///TODO 这里需要把Y轴翻转一次
    model->getQuad().mirrorVertical();
    return ret;
}

HwResult AlImageLayer::_measureOperate(AlImageLayerDrawModel &drawModel) {
    if (nullptr == model) {
        return Hw::FAILED;
    }
    for (auto *opt : model->operators) {
        if (opt) {
            opt->measure(drawModel);
        }
    }
    return Hw::SUCCESS;
}
