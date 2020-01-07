/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayerModel.h"
#include "AlMCropAction.h"
#include "AlMAlignCropAction.h"
#include "AlOperateFactory.h"
#include "StringUtils.h"
#include "Logcat.h"

#define TAG "AlImageLayerModel"
#define TAG_LAYER                           "layer"
#define TAG_SCALE                           "scale"
#define TAG_ROTATION                        "rotation"
#define TAG_POSITION                        "position"

#define VAL_PATH                            "path"
#define VAL_ID                              "id"
#define VAL_ALPHA                           "alpha"
#define VAL_RATIONAL_NUM                    "num"
#define VAL_RATIONAL_DEN                    "den"
#define VAL_VEC2_X                          "x"
#define VAL_VEC2_Y                          "y"

AlImageLayerModel *AlImageLayerModel::create(AlIdentityCreator *creator, const std::string path) {
    return create(creator->generate(), path);
}

AlImageLayerModel *AlImageLayerModel::create(int32_t id, const std::string path) {
    return new AlImageLayerModel(id, path);
}

AlImageLayerModel::AlImageLayerModel(int32_t id, const std::string &path)
        : AlAbsElemented(),
          path(path),
          id(id),
          alpha(1.0f),
          rotation(AlRational()),
          position(AlVec2(0.0f, 0.0f)),
          scale(AlVec2(1.0f, 1.0f)) {

}

AlImageLayerModel::AlImageLayerModel(const AlImageLayerModel &o)
        : AlAbsElemented(),
          id(o.id),
          path(o.path),
          alpha(o.alpha),
          rotation(o.rotation),
          position(o.position),
          scale(o.scale) {

}

AlImageLayerModel::~AlImageLayerModel() {
    auto itr = operators.begin();
    while (operators.end() != itr) {
        delete *itr;
        ++itr;
    }
    operators.clear();
}

std::string AlImageLayerModel::getPath() {
    return path;
}

int32_t AlImageLayerModel::getId() {
    return id;
}

void AlImageLayerModel::setAlpha(float alpha) {
    this->alpha = alpha;
}

float AlImageLayerModel::getAlpha() {
    return alpha;
}

void AlImageLayerModel::setScale(float scaleX, float scaleY) {
    scale.x = scaleX;
    scale.y = scaleY;
}

AlVec2 AlImageLayerModel::getScale() {
    return scale;
}

void AlImageLayerModel::setRotation(AlRational &r) {
    this->rotation.num = r.num;
    this->rotation.den = r.den;
}

AlRational AlImageLayerModel::getRotation() {
    return rotation;
}

void AlImageLayerModel::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

void AlImageLayerModel::postPosition(float x, float y) {
    position.x += x;
    position.y += y;
}

AlVec2 AlImageLayerModel::getPosition() {
    return position;
}

void AlImageLayerModel::setQuad(AlPointF leftTop,
                                AlPointF leftBottom,
                                AlPointF rightBottom,
                                AlPointF rightTop) {
    quad.setLeftTop(leftTop.x, leftTop.y);
    quad.setLeftBottom(leftBottom.x, leftBottom.y);
    quad.setRightBottom(rightBottom.x, rightBottom.y);
    quad.setRightTop(rightTop.x, rightTop.y);
}

AlQuad &AlImageLayerModel::getQuad() {
    return quad;
}

void AlImageLayerModel::dump() {
    Logcat::i(TAG, "+--------------------------+");
    Logcat::i(TAG, "layer: %d", id);
    Logcat::i(TAG, "alpha: %f", alpha);
    Logcat::i(TAG, "scale: %f, %f", scale.x, scale.y);
    Logcat::i(TAG, "rotat: %f", rotation.toFloat());
    Logcat::i(TAG, "posit: %f, %f", position.x, position.y);
    Logcat::i(TAG, "quad : [%f,%f]-----[%f,%f]",
              quad.leftTop().x, quad.leftTop().y,
              quad.rightTop().x, quad.rightTop().y);
    Logcat::i(TAG, "quad : [%f,%f]-----[%f,%f]",
              quad.leftBottom().x, quad.leftBottom().y,
              quad.rightBottom().x, quad.rightBottom().y);
    Logcat::i(TAG, "+--------------------------+");
}

HwResult AlImageLayerModel::addOperator(AlAbsMAction *opt) {
    operators.push_back(opt);
    return Hw::SUCCESS;
}

std::vector<AlAbsMAction *> *AlImageLayerModel::getAllOperators() {
    return &operators;
}

bool AlImageLayerModel::removeCropOperator() {
    const type_info &info = typeid(AlMCropAction);
    return _removeOperator(info);
}

bool AlImageLayerModel::removeAlignCropOperator() {
    const type_info &info = typeid(AlMAlignCropAction);
    return _removeOperator(info);
}

bool AlImageLayerModel::_removeOperator(const type_info &info) {
    bool ret = false;
    auto itr = operators.begin();
    while (itr != operators.end()) {
        AlAbsMAction *it = *itr;
        if (info == typeid(*it)) {
            itr = operators.erase(itr);
            ret = true;
            continue;
        }
        ++itr;
    }
    return ret;
}

HwResult AlImageLayerModel::fromElement(AlElement *element) {
    if (nullptr == element) {
        return Hw::FAILED;
    }
    std::string name = element->name();
    if (!element->nameIs(TAG_LAYER)) {
        return Hw::FAILED;
    }
    path = element->attr(VAL_PATH);
    id = element->attrInt(VAL_ID);
    alpha = element->attrFloat(VAL_ALPHA);
    if (StringUtils::isEmpty(&path) || alpha > 1 || alpha < -1) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    size_t size = element->size();
    for (int i = 0; i < size; ++i) {
        AlElement *child = element->childAt(i);
        if (child->nameIs(TAG_SCALE)) {
            this->scale.x = child->attrFloat(VAL_VEC2_X);
            this->scale.y = child->attrFloat(VAL_VEC2_Y);
        } else if (child->nameIs(TAG_ROTATION)) {
            this->rotation.num = child->attrInt(VAL_RATIONAL_NUM);
            this->rotation.den = child->attrInt(VAL_RATIONAL_DEN);
        } else if (child->nameIs(TAG_POSITION)) {
            this->position.x = child->attrFloat(VAL_VEC2_X);
            this->position.y = child->attrFloat(VAL_VEC2_Y);
        } else if (child->nameIs(TAG_ACTION)) {
            std::string type = child->attr(VAL_TYPE);
            AlAbsMAction *model = nullptr;
            if (StringUtils::equalsIgnoreCase(AlAbsMAction::TYPE_CROP, type)) {
                model = AlOperateFactory::crop(MAXFLOAT, MAXFLOAT, MAXFLOAT, MAXFLOAT);
            } else if (StringUtils::equalsIgnoreCase(AlAbsMAction::TYPE_ALIGN_CROP, type)) {
                AlRational r(INT32_MIN, INT32_MIN);
                model = AlOperateFactory::alignCrop(r);
            }
            if (model && Hw::SUCCESS == model->fromElement(child)) {
                addOperator(model);
            } else {
                delete model;
            }
        }
    }
    return Hw::SUCCESS;
}

HwResult AlImageLayerModel::toElement(AlElement **element) {
    AlElement *root = new AlElement(TAG_LAYER);
    root->addAttr(VAL_PATH, path);
    root->addAttr(VAL_ID, StringUtils::valueOf(id));
    root->addAttr(VAL_ALPHA, StringUtils::valueOf(alpha));
    *element = root;

    AlElement *scale = new AlElement(TAG_SCALE);
    scale->addAttr(VAL_VEC2_X, StringUtils::valueOf(this->scale.x));
    scale->addAttr(VAL_VEC2_Y, StringUtils::valueOf(this->scale.y));

    AlElement *rotation = new AlElement(TAG_ROTATION);
    rotation->addAttr(VAL_RATIONAL_NUM, StringUtils::valueOf(this->rotation.num));
    rotation->addAttr(VAL_RATIONAL_DEN, StringUtils::valueOf(this->rotation.den));

    AlElement *pos = new AlElement(TAG_POSITION);
    pos->addAttr(VAL_VEC2_X, StringUtils::valueOf(this->position.x));
    pos->addAttr(VAL_VEC2_Y, StringUtils::valueOf(this->position.y));

    root->addChild(scale);
    root->addChild(rotation);
    root->addChild(pos);

    size_t size = operators.size();
    for (int i = 0; i < size; ++i) {
        AlElement *opt = nullptr;
        operators[i]->toElement(&opt);
        if (opt) {
            root->addChild(opt);
        }
    }
    return Hw::SUCCESS;
}
