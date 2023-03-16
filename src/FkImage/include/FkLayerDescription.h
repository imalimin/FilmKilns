/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-12 00:11:02
*/

#ifndef FK_IMAGE_FKLAYERDESCRIPTION_H
#define FK_IMAGE_FKLAYERDESCRIPTION_H

#include "FkObject.h"
#include "FkDeviceImage.h"

FK_SUPER_CLASS(FkLayerDescription, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkLayerDescription)

public:
    FkLayerDescription();

    FkLayerDescription(const FkLayerDescription &o);

    virtual ~FkLayerDescription();

public:
    std::shared_ptr<FkDeviceImage> deviceImage = nullptr;
};

#endif //FK_IMAGE_FKLAYERDESCRIPTION_H