/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGELAYERMODEL_H
#define HWVC_ANDROID_ALIMAGELAYERMODEL_H

#include "Object.h"

al_class(AlImageLayerModel) {
public:
    static AlImageLayerModel *create(const std::string path);

private:
    AlImageLayerModel(const std::string &path);

public:

    AlImageLayerModel(const AlImageLayerModel &e);

    ~AlImageLayerModel();

    std::string getPath();

private:
    std::string path;
};


#endif //HWVC_ANDROID_ALIMAGELAYERMODEL_H
