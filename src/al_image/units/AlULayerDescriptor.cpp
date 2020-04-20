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
    _updateCanvasSize(msg->arg1, static_cast<int>(msg->arg2));
    return true;
}

HwResult AlULayerDescriptor::_measure(AlImageLayer *layer,
                                      AlImageLayerModel *layerModel,
                                      AlImageLayerDrawModel *description) {
    if (nullptr == layer || nullptr == layerModel || nullptr == description) {
        return Hw::FAILED;
    }
    AlSize layerSize(layer->getWidth(), layer->getHeight());
    ///默认画布大小为最先添加的图层的大小
    if (aCanvasSize.width <= 0 || aCanvasSize.height <= 0) {
        _updateCanvasSize(layerSize.width, layerSize.height);
    }
    ///Copy一份layer model送入opt进行测量，在测量过程中opt可能会改变model数据
    AlImgLayerDescription model(*(layerModel));
    model.setSize(layerSize);
    HwResult ret = _measureOperate(layerModel->getAllActions(), model, description);
    _measureLayerQuad(model);

    layerModel->getQuad().dump();
    layerModel->setQuad(model.getQuad());
    description->vertex = model.getQuad();
    description->alpha = model.getAlpha();
    description->tex = HwTexture::wrap(dynamic_cast<HwTexture *>(layer->getTexture()));
    description->setLayerSize(model.getSize());
    return ret;
}

void AlULayerDescriptor::_measureLayerQuad(AlImgLayerDescription &model) {
    ///经过各种各样的Operate后，layer size会被改变并更新到AlImageLayerDrawModel
    ///这里需要获取最新的layer size，不然会出错
    ///必须裁剪Operate会改变layer size，如果不更新则可能出现图像拉伸
    AlSize size = model.getSize();
    AlRectF rect(-size.width / 2,
                 size.height / 2,
                 size.width / 2,
                 -size.height / 2);
    oMat.reset();
    oMat.update(-aCanvasSize.width / 2.f, aCanvasSize.width / 2.f,
                -aCanvasSize.height / 2.f, aCanvasSize.height / 2.f, -1.0f, 1.0f);

    AlVec4 lt(rect.left, rect.top);
    AlVec4 rt(rect.right, rect.top);
    AlVec4 rb(rect.right, rect.bottom);
    AlVec4 lb(rect.left, rect.bottom);
    tMat.reset();
    tMat.setScale(model.getScale().x, model.getScale().y);
    tMat.setRotation(static_cast<float>(-model.getRadian()));
    tMat.setTranslate(model.getPosition().x * aCanvasSize.width / 2.f,
                      model.getPosition().y * aCanvasSize.height / 2.f);
    lt = lt * tMat;
    rt = rt * tMat;
    rb = rb * tMat;
    lb = lb * tMat;

    AlVec2 lt2 = (lt * oMat).xy();
    AlVec2 rt2 = (rt * oMat).xy();
    AlVec2 rb2 = (rb * oMat).xy();
    AlVec2 lb2 = (lb * oMat).xy();
    model.setQuad(lt2, lb2, rb2, rt2);
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
//    Logcat::i(TAG, "%s(%d): %d", __FUNCTION__, __LINE__, flags);
    AlRenderParams params(flags);
    if (params.isReqClear()) {
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

void AlULayerDescriptor::_updateCanvasSize(int32_t width, int32_t height) {
    aCanvasSize.width = width;
    aCanvasSize.height = height;
}
