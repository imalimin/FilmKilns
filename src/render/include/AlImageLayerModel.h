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

private:
    int32_t id = AlIdentityCreator::NONE_ID;
    std::string path;
    float alpha = 1.0f;
    float rotation = 0.0f;
    AlVec2 position;
    AlVec2 scale;
};


#endif //HWVC_ANDROID_ALIMAGELAYERMODEL_H
