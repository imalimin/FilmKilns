/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGELAYERMODEL_H
#define HWVC_ANDROID_ALIMAGELAYERMODEL_H

#include "Object.h"
#include "AlVec2.h"
#include "AlIdentityCreator.h"
#include "AlQuad.h"
#include "AlRational.h"

al_class(AlImageLayerModel) {
public:
    static AlImageLayerModel *create(AlIdentityCreator *creator, const std::string path);

private:
    AlImageLayerModel(int32_t id, const std::string &path);

public:

    AlImageLayerModel(const AlImageLayerModel &o);

    ~AlImageLayerModel();

    std::string getPath();

    int32_t getId();

    void setAlpha(float alpha);

    float getAlpha();

    void setScale(float scaleX, float scaleY);

    AlVec2 getScale();

    void setRotation(AlRational &r);

    AlRational getRotation();

    void postPosition(float x, float y);

    void setPosition(float x, float y);

    AlVec2 getPosition();

    void setQuad(AlPointF leftTop, AlPointF leftBottom, AlPointF rightBottom, AlPointF rightTop);

    AlQuad &getQuad();

    void dump();

private:
    std::string path;
    int32_t id = AlIdentityCreator::NONE_ID;
    float alpha = 1.0f;
    ///弧度表示，顺时针方向为正向(CW)，r=rotation*PI
    AlRational rotation = AlRational();
    AlVec2 position;
    AlVec2 scale;
    AlQuad quad;
};


#endif //HWVC_ANDROID_ALIMAGELAYERMODEL_H
