/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-14 23:34:11
*/

#ifndef FK_GRAPHIC_FKDEVICEIMAGECOMPO_H
#define FK_GRAPHIC_FKDEVICEIMAGECOMPO_H

#include "FkComponent.h"
#include "FkDeviceImage.h"

FK_SUPER_CLASS(FkDeviceImageCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkDeviceImageCompo)

public:
    FkDeviceImageCompo(std::shared_ptr<FkDeviceImage> &deviceImage);

    FkDeviceImageCompo(const FkDeviceImageCompo &o);

    virtual ~FkDeviceImageCompo();

public:
    std::shared_ptr<FkDeviceImage> deviceImage = nullptr;
};

#endif //FK_GRAPHIC_FKDEVICEIMAGECOMPO_H