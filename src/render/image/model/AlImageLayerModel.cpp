/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageLayerModel.h"
#include "Logcat.h"

#define TAG "AlImageLayerModel"

AlImageLayerModel *AlImageLayerModel::create(AlIdentityCreator *creator, const std::string path) {
    return new AlImageLayerModel(creator->generate(), path);
}

AlImageLayerModel::AlImageLayerModel(int32_t id, const std::string &path)
        : Object(),
          path(path),
          id(id),
          alpha(1.0f),
          rotation(0.0f),
          position(AlVec2(0.0f, 0.0f)),
          scale(AlVec2(1.0f, 1.0f)) {

}

AlImageLayerModel::AlImageLayerModel(const AlImageLayerModel &o)
        : Object(),
          id(o.id),
          path(o.path),
          alpha(o.alpha),
          rotation(o.rotation),
          position(o.position),
          scale(o.scale) {

}

AlImageLayerModel::~AlImageLayerModel() {

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

void AlImageLayerModel::setRotation(float rotation) {
    this->rotation = rotation;
}

float AlImageLayerModel::getRotation() {
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
    Logcat::i(TAG, "rotat: %f", rotation);
    Logcat::i(TAG, "posit: %f, %f", position.x, position.y);
    Logcat::i(TAG, "quad : [%f,%f]-----[%f,%f]",
              quad.leftTop().x, quad.leftTop().y,
              quad.rightTop().x, quad.rightTop().y);
    Logcat::i(TAG, "quad : [%f,%f]-----[%f,%f]",
              quad.leftBottom().x, quad.leftBottom().y,
              quad.rightBottom().x, quad.rightBottom().y);
    Logcat::i(TAG, "+--------------------------+");
}
