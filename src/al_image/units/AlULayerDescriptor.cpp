/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlULayerDescriptor.h"
#include "AlImageLayerDrawModel.h"
#include "AlMath.h"
#include "HwTexture.h"
#include "ObjectBox.h"
#include "AlImgLayerDescription.h"
#include "AlAbsMAction.h"
#include "AlLayerPair.h"
#include "AlRenderParams.h"

#define TAG "AlULayerDescriptor"

AlULayerDescriptor::AlULayerDescriptor(const string &alias) : Unit(alias) {
    registerEvent(EVENT_LAYER_MEASURE, reinterpret_cast<EventFunc>(&AlULayerDescriptor::onMeasure));
    registerEvent(EVENT_LAYER_MEASURE_CANVAS_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlULayerDescriptor::onCanvasSizeUpdate));
}

AlULayerDescriptor::~AlULayerDescriptor() {

}

bool AlULayerDescriptor::onCreate(AlMessage *msg) {
    return true;
}

bool AlULayerDescriptor::onDestroy(AlMessage *msg) {
    return true;
}

bool AlULayerDescriptor::onMeasure(AlMessage *msg) {
    AlLayerPair *pair = msg->getObj<AlLayerPair *>();
    AlImageLayerDrawModel *description = new AlImageLayerDrawModel();
    _measure(pair->layer, pair->model, description);
    notifyCanvas(description, msg->arg1);
    return true;
}

bool AlULayerDescriptor::onCanvasSizeUpdate(AlMessage *msg) {
    aCanvasSize.width = msg->arg1;
    aCanvasSize.height = static_cast<int>(msg->arg2);
    return true;
}

HwResult AlULayerDescriptor::_measure(AlImageLayer *layer,
                                      AlImageLayerModel *layerModel,
                                      AlImageLayerDrawModel *description) {
    if (nullptr == layer || nullptr == layerModel || nullptr == description) {
        return Hw::FAILED;
    }
    AlSize layerSize(layer->getWidth(), layer->getHeight());
    AlSize canvasSize = aCanvasSize;
    ///默认画布大小为最先添加的图层的大小
    if (canvasSize.width <= 0 || canvasSize.height <= 0) {
        canvasSize.width = layerSize.width;
        canvasSize.height = layerSize.height;
    }
    ///Copy一份layer model送入opt进行测量，在测量过程中opt可能会改变model数据
    AlImgLayerDescription model(*(layerModel));
    model.setSize(layerSize);
    HwResult ret = _measureOperate(layerModel->getAllActions(), model, description);
    description->setLayerSize(model.getSize());
    ///经过各种各样的Operate后，layer size会被改变并更新到AlImageLayerDrawModel
    ///这里需要获取最新的layer size，不然会出错
    ///必须裁剪Operate会改变layer size，如果不更新则可能出现图像拉伸
    AlSize src = description->getLayerSize();
    /// 对图层和画布进行正交投影计算，转换坐标系，保证图像旋转缩放不会变形，并得到归一化的区域
    aMeasurer.updateOrthogonal(src, canvasSize);
    /// 缩放旋转位移顺序不能乱
    aMeasurer.setScale(model.getScale().x, model.getScale().y);
    aMeasurer.setRotation(static_cast<float>(model.getRotation().toFloat() * AlMath::PI));
    ///TODO 矩阵Y轴与正常坐标系Y轴相反
    aMeasurer.setTranslate(model.getPosition().x, -model.getPosition().y);
    aMeasurer.measure(*description);
    description->tex = HwTexture::wrap(dynamic_cast<HwTexture *>(layer->getTexture()));
    description->alpha = model.getAlpha();
    ///Update quad.
    AlVec2 lt;
    AlVec2 lb;
    AlVec2 rb;
    AlVec2 rt;
    ///获得经过位移旋转缩放变换后图像的位置坐标
    aMeasurer.measureTransLORectF(lt, lb, rb, rt);
    layerModel->setQuad(lt, lb, rb, rt);
    ///TODO 这里需要把Y轴翻转一次
    layerModel->getQuad().mirrorVertical();
    Logcat::i(TAG, "tran %f, %f", model.getPosition().x, model.getPosition().y);
    Logcat::i(TAG, "rect (%f,%f), (%f,%f)", lt.x, lt.y, rt.x, rt.y);
    Logcat::i(TAG, "rect (%f,%f), (%f,%f)", lb.x, lb.y, rb.x, rb.y);
    return ret;
}

HwResult AlULayerDescriptor::_measureOperate(std::vector<AlAbsMAction *> *opts,
                                             AlImgLayerDescription &model,
                                             AlImageLayerDrawModel *description) {
    if (nullptr == description) {
        return Hw::FAILED;
    }
    for (auto *opt : *opts) {
        if (opt) {
            opt->setCanvasSize(aCanvasSize);
            opt->measure(model, description);
        }
    }
    return Hw::SUCCESS;
}

void AlULayerDescriptor::notifyCanvas(AlImageLayerDrawModel *description, int32_t flags) {
    Logcat::i(TAG, "%s(%d): %d", __FUNCTION__, __LINE__, flags);
    AlRenderParams params(flags);
    if(params.isReqClear()) {
        AlMessage *msg = AlMessage::obtain(EVENT_LAYER_RENDER_CLEAR);
        msg->arg1 = params.isTransparent();
        msg->desc = "clear";
        postEvent(msg);
    }
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_RENDER_DRAW, description);
    msg->arg1 = flags;
    postEvent(msg);
    if (params.isRenderScreen()) {
        AlMessage *sMsg = AlMessage::obtain(EVENT_LAYER_RENDER_SHOW);
        sMsg->desc = "show";
        postEvent(sMsg);
    }
}
