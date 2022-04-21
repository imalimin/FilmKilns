/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-21 23:01:50
*/

#ifndef FK_BITMAP_FKIMAGE_H
#define FK_BITMAP_FKIMAGE_H

#include "FkObject.h"

FK_CLASS FkImage FK_EXTEND FkObject {
public:
    FK_ENUM Format {
        kBMP = 1,
        kGIF,
        kICO,
        kJPEG,
        kPNG,
        kWBMP,
        kWEBP,
        kPKM,
        kKTX,
        kASTC,
        kDNG,
        kHEIF,
    };
public:
    FkImage();

    FkImage(const FkImage &o);

    virtual ~FkImage();
};

#endif //FK_BITMAP_FKIMAGE_H