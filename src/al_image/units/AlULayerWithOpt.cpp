/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlULayerWithOpt.h"
#include "AlPaintDesc.h"
#include "AlMPaintAction.h"
#include "AlLayerActionFactory.h"
#include "AlOperateScale.h"
#include "AlOperateRotate.h"
#include "AlOperateTrans.h"
#include "AlOperateAlpha.h"
#include "AlOperateCrop.h"

AlULayerWithOpt::AlULayerWithOpt(string alias) : AlULayer(alias) {
    registerEvent(EVENT_LAYER_QUERY,
                  reinterpret_cast<EventFunc>(&AlULayerWithOpt::onOperateQuery));
    registerEvent(EVENT_LAYER_PAINT, reinterpret_cast<EventFunc>(&AlULayerWithOpt::onOperatePaint));
    registerEvent(EVENT_LAYER_SCALE, reinterpret_cast<EventFunc>(&AlULayerWithOpt::onOperateScale));
    registerEvent(EVENT_LAYER_SCALE_POST,
                  reinterpret_cast<EventFunc>(&AlULayerWithOpt::onOperatePostScale));
    registerEvent(EVENT_LAYER_ROTATE,
                  reinterpret_cast<EventFunc>(&AlULayerWithOpt::onOperateRotate));
    registerEvent(EVENT_LAYER_ROTATE_POST,
                  reinterpret_cast<EventFunc>(&AlULayerWithOpt::onOperatePostRotate));
    registerEvent(EVENT_LAYER_TRANS, reinterpret_cast<EventFunc>(&AlULayerWithOpt::onOperateTrans));
    registerEvent(EVENT_LAYER_TRANS_POST,
                  reinterpret_cast<EventFunc>(&AlULayerWithOpt::onOperatePostTrans));
    registerEvent(EVENT_LAYER_ALPHA,
                  reinterpret_cast<EventFunc>(&AlULayerWithOpt::onOperateAlpha));
    registerEvent(EVENT_LAYER_CROP,
                  reinterpret_cast<EventFunc>(&AlULayerWithOpt::onCropLayer));
    registerEvent(EVENT_LAYER_CROP_CANCEL,
                  reinterpret_cast<EventFunc>(&AlULayerWithOpt::onCropLayerCancel));
    registerEvent(EVENT_CANVAS_CROP,
                  reinterpret_cast<EventFunc>(&AlULayerWithOpt::onCropCanvas));
}

AlULayerWithOpt::~AlULayerWithOpt() {

}

bool AlULayerWithOpt::onOperatePaint(AlMessage *m) {
    auto *desc = m->getObj<AlPaintDesc *>();
    if (nullptr == desc) {
        return true;
    }
    auto *layer = findLayerModel(desc->layerId);
    if (layer) {
        /// 由于OpenGL绘制点shader点坐标和纹理坐标不一样，所以这里y取反
        AlVec2 pointF = transWin2Layer(layer, desc->point.x, -desc->point.y);
        AlAbsMAction *action = nullptr;
        auto *actions = layer->getAllActions();
        size_t size = actions->size();
        for (int i = 0; i < size; ++i) {
            AlAbsMAction *a = (*actions)[i];
            if (typeid(AlMPaintAction) == typeid(*a)) {
                action = a;
            }
        }
        if (nullptr == action) {
            action = AlLayerActionFactory::paint(0.01f, AlColor(0xff0000));
            layer->addAction(action);
        }
        dynamic_cast<AlMPaintAction *>(action)->paint(pointF);
        if (!desc->painting) {
            dynamic_cast<AlMPaintAction *>(action)->newPath();
        }
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onOperateScale(AlMessage *m) {
    auto *desc = m->getObj<AlOperateScale *>();
    if (nullptr == desc) {
        return true;
    }
    auto *model = findLayerModel(desc->layerId);
    if (model) {
        model->setScale(desc->scale.toFloat(), desc->scale.toFloat());
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onOperatePostScale(AlMessage *m) {
    auto *desc = m->getObj<AlOperateScale *>();
    if (nullptr == desc) {
        return true;
    }
    auto *model = findLayerModel(desc->layerId);
    if (model) {
        AlVec2 vec = transWin2Canvas(desc->anchor.x, desc->anchor.y);
        float scale = desc->scale.toFloat();
        float dx = desc->anchor.x - model->getPosition().x;
        float dy = desc->anchor.y - model->getPosition().y;
        float x = dx * (1.0f - scale);
        float y = dy * (1.0f - scale);
        model->setPosition(model->getPosition().x + x, model->getPosition().y + y);
        model->setScale(model->getScale().x * scale,
                        model->getScale().y * scale);
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onOperateRotate(AlMessage *m) {
    auto *desc = m->getObj<AlOperateRotate *>();
    if (nullptr == desc) {
        return true;
    }
    auto *model = findLayerModel(desc->layerId);
    if (model) {
        model->setRotation(desc->rotation);
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onOperatePostRotate(AlMessage *m) {
    auto *desc = m->getObj<AlOperateRotate *>();
    if (nullptr == desc) {
        return true;
    }
    auto *model = findLayerModel(desc->layerId);
    if (model) {
        AlVec2 vec = transWin2Canvas(desc->anchor.x, desc->anchor.y);
        ///TODO 还可以提高精度
        auto nr = model->getRotation().toFloat() + desc->rotation.toFloat();
        auto rotation = AlRational(static_cast<int32_t>(nr * 100000), 100000);
        model->setRotation(rotation);
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onOperateTrans(AlMessage *m) {
    auto *desc = m->getObj<AlOperateTrans *>();
    if (nullptr == desc) {
        return true;
    }
    auto *model = findLayerModel(desc->layerId);
    if (model) {
        AlVec2 vec = transWin2Canvas(desc->x, desc->y);
        model->setPosition(vec.x, vec.y);
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onOperatePostTrans(AlMessage *m) {
    auto *desc = m->getObj<AlOperateTrans *>();
    if (nullptr == desc) {
        return true;
    }
    auto model = findLayerModel(desc->layerId);
    if (model) {
        AlVec2 vec = transWin2Canvas(desc->x, desc->y);
        model->setPosition(model->getPosition().x + vec.x, model->getPosition().y + vec.y);
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onOperateQuery(AlMessage *m) {
    auto *desc = m->getObj<AlOperateTrans *>();
    if (nullptr == desc) {
        return true;
    }
    auto *model = findLayerModel(desc->x, desc->y);
    auto *msg = AlMessage::obtain(EVENT_LAYER_QUERY_NOTIFY);
    msg->arg1 = nullptr != model ? model->getId() : AlIdentityCreator::NONE_ID;
    postEvent(msg);
    return true;
}

bool AlULayerWithOpt::onOperateAlpha(AlMessage *m) {
    auto *desc = m->getObj<AlOperateAlpha *>();
    if (nullptr == desc) {
        return true;
    }
    auto model = findLayerModel(desc->layerId);
    if (model) {
        model->setAlpha(desc->alpha);
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onCropLayer(AlMessage *m) {
    auto *desc = m->getObj<AlOperateCrop *>();
    if (nullptr == desc) {
        return true;
    }
    auto model = findLayerModel(desc->layerId);
    if (model) {
        AlVec2 lt = transWin2Canvas(desc->rectF.left, desc->rectF.top);
        AlVec2 rb = transWin2Canvas(desc->rectF.right, desc->rectF.bottom);
        model->removeCropAction();
        model->addAction(AlLayerActionFactory::crop(lt.x, lt.y, rb.x, rb.y));
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onCropLayerCancel(AlMessage *m) {
    auto *desc = m->getObj<AlOperateCrop *>();
    if (nullptr == desc) {
        return true;
    }
    auto model = findLayerModel(desc->layerId);
    if (model && model->removeCropAction()) {
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onCropCanvas(AlMessage *m) {
    auto *desc = m->getObj<AlOperateCrop *>();
    if (nullptr == desc) {
        return true;
    }
    AlVec2 lt = transWin2Canvas(desc->rectF.left, desc->rectF.top);
    AlVec2 rb = transWin2Canvas(desc->rectF.right, desc->rectF.bottom);
    AlRectF rectF(lt.x, lt.y, rb.x, rb.y);
    AlSize src = getCanvasSize();
    AlSize dst(src.width * rectF.getWidth() / 2,
               src.height * rectF.getHeight() / 2);
    AlPointF anchor(-(rectF.right + rectF.left) / 2.0f, -(rectF.top + rectF.bottom) / 2.0f);
    cropCanvasAndStayLoc(&src, &dst, &anchor);
    postEvent(AlMessage::obtain(EVENT_CANVAS_RESIZE, new AlSize(dst)));
    invalidate();
    return true;
}
