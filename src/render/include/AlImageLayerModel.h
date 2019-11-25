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

al_class(AlImageLayerModel) {
public:
    static AlImageLayerModel *create(const std::string path);

private:
    AlImageLayerModel(int32_t id, const std::string &path);

public:

    AlImageLayerModel(const AlImageLayerModel &o);

    ~AlImageLayerModel();

    std::string getPath();

private:
    int32_t id = 0;
    std::string path;
    float alpha = 1.0f;
    float rotation = 0.0f;
    AlVec2 position;
    AlVec2 scale;
};


#endif //HWVC_ANDROID_ALIMAGELAYERMODEL_H
