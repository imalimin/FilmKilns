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
#include "AlMath.h"

AlULayerWithOpt::AlULayerWithOpt(string alias) : AlULayer(alias) {
    al_reg_msg(EVENT_LAYER_QUERY_ID, AlULayerWithOpt::onOperateQuery);
    al_reg_msg(EVENT_LAYER_PAINT, AlULayerWithOpt::onOperatePaint);
    al_reg_msg(EVENT_LAYER_SCALE, AlULayerWithOpt::onOperateScale);
    al_reg_msg(EVENT_LAYER_SCALE_POST, AlULayerWithOpt::onOperatePostScale);
    al_reg_msg(EVENT_LAYER_ROTATE, AlULayerWithOpt::onOperateRotate);
    al_reg_msg(EVENT_LAYER_ROTATE_POST, AlULayerWithOpt::onOperatePostRotate);
    al_reg_msg(EVENT_LAYER_TRANS, AlULayerWithOpt::onOperateTrans);
    al_reg_msg(EVENT_LAYER_TRANS_POST, AlULayerWithOpt::onOperatePostTrans);
    al_reg_msg(EVENT_LAYER_ALPHA, AlULayerWithOpt::onOperateAlpha);
    al_reg_msg(EVENT_LAYER_CROP, AlULayerWithOpt::onCropLayer);
    al_reg_msg(EVENT_LAYER_CROP_CANCEL, AlULayerWithOpt::onCropLayerCancel);
    al_reg_msg(EVENT_LAYER_ALIGN_CROP, AlULayerWithOpt::onAlignCropLayer);
    al_reg_msg(EVENT_LAYER_ALIGN_CROP_CANCEL, AlULayerWithOpt::onAlignCropLayerCancel);
    al_reg_msg(MSG_LAYER_PAINT_POINT, AlULayerWithOpt::onOperatePaintPoint);
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
        AlVec2 pointF = transWin2Layer(layer, desc->point.x, desc->point.y);
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
        model->setScale(desc->scaleX.toFloat(), desc->scaleY.toFloat());
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
        float scaleX = desc->scaleX.toFloat();
        float scaleY = desc->scaleY.toFloat();
        float dx = desc->anchor.x - model->getPosition().x;
        float dy = desc->anchor.y - model->getPosition().y;
        float x = dx * (1.0f - scaleX);
        float y = dy * (1.0f - scaleY);
        model->setPosition(model->getPosition().x + x, model->getPosition().y + y);
        model->setScale(model->getScale().x * scaleX,
                        model->getScale().y * scaleY);
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
        AlVec2 anchor = transWin2Canvas(desc->anchor.x, desc->anchor.y);
//        AlLogI("alimin", "%f, %f", anchor.x, anchor.y);
        AlVec4 vec4(anchor.x - model->getPosition().x,
                    anchor.y - model->getPosition().y);
        AlMatrix mat;
        mat.setRotation(static_cast<float>(desc->rotation.toFloat() * AlMath::PI));
        vec4 = vec4 * mat;
//        vec4.x += model->getPosition().x;
//        vec4.y += model->getPosition().y;

        model->setPosition(anchor.x + vec4.x,
                           anchor.y - vec4.y);
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
    auto *msg = AlMessage::obtain(EVENT_LAYER_QUERY_ID_NOTIFY);
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

bool AlULayerWithOpt::onAlignCropLayer(AlMessage *m) {
    auto *desc = m->getObj<AlOperateRotate *>();
    if (nullptr == desc) {
        return true;
    }
    auto model = findLayerModel(desc->layerId);
    if (model) {
        model->removeAlignCropAction();
        model->addAction(AlLayerActionFactory::alignCrop(desc->rotation));
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onAlignCropLayerCancel(AlMessage *m) {
    auto *desc = m->getObj<AlOperateRotate *>();
    if (nullptr == desc) {
        return true;
    }
    auto model = findLayerModel(desc->layerId);
    if (model && model->removeAlignCropAction()) {
        invalidate();
    }
    return true;
}

bool AlULayerWithOpt::onOperatePaintPoint(AlMessage *m) {
    auto *desc = m->getObj<AlPaintDesc *>();
    if (nullptr == desc) {
        return true;
    }
    auto *layer = findLayerModel(desc->layerId);
    if (layer) {
    }
    return true;
}
