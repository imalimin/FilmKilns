/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlLayerDescriptor.h"
#include "AlImageLayerDrawModel.h"
#include "AlMath.h"
#include "HwTexture.h"
#include "ObjectBox.h"

AlLayerDescriptor::AlLayerDescriptor(const string &alias) : Unit(alias) {
    registerEvent(EVENT_LAYER_MEASURE, reinterpret_cast<EventFunc>(&AlLayerDescriptor::onMeasure));
    registerEvent(EVENT_LAYER_MEASURE_CANVAS_SIZE,
                  reinterpret_cast<EventFunc>(&AlLayerDescriptor::onCanvasSizeUpdate));
}

AlLayerDescriptor::~AlLayerDescriptor() {

}

bool AlLayerDescriptor::onCreate(Message *msg) {
    return true;
}

bool AlLayerDescriptor::onDestroy(Message *msg) {
    return true;
}

bool AlLayerDescriptor::onMeasure(Message *msg) {
    AlImageLayer *layer = msg->getObj<ObjectBox *>()->unWrap<AlImageLayer *>();
    AlImageLayerDrawModel *description = new AlImageLayerDrawModel();
    _measure(layer, description);
    notifyCanvas(description);
    return true;
}

bool AlLayerDescriptor::onCanvasSizeUpdate(Message *msg) {
    aCanvasSize.width = msg->arg1;
    aCanvasSize.height = static_cast<int>(msg->arg2);
    return true;
}

HwResult AlLayerDescriptor::_measure(AlImageLayer *layer, AlImageLayerDrawModel *description) {
    if (nullptr == layer || nullptr == layer->model || nullptr == description) {
        return Hw::FAILED;
    }
    ///先把图层原始大小保存到AlImageLayerDrawModel，各种Operate中回用到
    description->setLayerSize(layer->getWidth(), layer->getHeight());
    HwResult ret = _measureOperate(layer, description);
    AlSize canvasSize = aCanvasSize;
    ///默认画布大小为最先添加的图层的大小
    if (canvasSize.width <= 0 || canvasSize.height <= 0) {
        canvasSize.width = layer->getWidth();
        canvasSize.height = layer->getHeight();
    }
    ///经过各种各样的Operate后，layer size会被改变并更新到AlImageLayerDrawModel
    ///这里需要获取最新的layer size，不然会出错
    ///必须裁剪Operate会改变layer size，如果不更新则可能出现图像拉伸
    AlSize src = description->getLayerSize();
    /// 对图层和画布进行正交投影计算，转换坐标系，保证图像旋转缩放不会变形，并得到归一化的区域
    aMeasure.updateOrthogonal(src, canvasSize);
    aMeasure.setScale(layer->model->getScale().x, layer->model->getScale().y);
    aMeasure.setRotation(static_cast<float>(layer->model->getRotation().toFloat() * AlMath::PI));
    ///TODO 矩阵Y轴与正常坐标系Y轴相反
    aMeasure.setTranslate(layer->model->getPosition().x, -layer->model->getPosition().y);
    aMeasure.measure(*description);
    description->tex = HwTexture::wrap(dynamic_cast<HwTexture *>(layer->getTexture()));
    description->alpha = layer->model->getAlpha();
    ///Update quad.
    AlVec2 lt;
    AlVec2 lb;
    AlVec2 rb;
    AlVec2 rt;
    ///获得经过位移旋转缩放变换后图像的位置坐标
    aMeasure.getTransLORectF(lt, lb, rb, rt);
    layer->model->setQuad(lt, lb, rb, rt);
    ///TODO 这里需要把Y轴翻转一次
    layer->model->getQuad().mirrorVertical();
    return ret;
}

HwResult
AlLayerDescriptor::_measureOperate(AlImageLayer *layer, AlImageLayerDrawModel *description) {
    if (nullptr == layer || nullptr == description) {
        return Hw::FAILED;
    }
    AlAbsOperateModel::AlLayerDesc desc;
    desc.rotation = layer->model->getRotation();
    for (auto *opt : *layer->model->getAllOperators()) {
        if (opt) {
            opt->measure(desc, *description);
        }
    }
    return Hw::SUCCESS;
}

void AlLayerDescriptor::notifyCanvas(AlImageLayerDrawModel *description) {
    Message *msg = new Message(EVENT_LAYER_RENDER_DRAW, description,
                               Message::QUEUE_MODE_FIRST_ALWAYS);
    postEvent(msg);
}
