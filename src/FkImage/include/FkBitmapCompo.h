/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-5 0:43:52
*/

#ifndef FK_IMAGE_FKBITMAPCOMPO_H
#define FK_IMAGE_FKBITMAPCOMPO_H

#include "FkComponent.h"
#include "FkBitmap.h"

FK_CLASS FkBitmapCompo FK_EXTEND FkComponent {
public:
    FkBitmapCompo(std::shared_ptr<FkBitmap> bmp);

    FkBitmapCompo(const FkBitmapCompo &o);

    virtual ~FkBitmapCompo();

public:
    std::shared_ptr<FkBitmap> bmp = nullptr;
};

#endif //FK_IMAGE_FKBITMAPCOMPO_H