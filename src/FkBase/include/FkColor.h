/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKCOLOR_H
#define FK_BASE_FKCOLOR_H

#include "FkObject.h"

FK_CLASS FkColor FK_EXTEND FkObject {
public:
    FK_ENUM kFormat : uint32_t {
        NONE = 0,
        LUMINANCE,
        LUMINANCE2,
        RGB,
        RGBA,
        RGB565,
    };
public:
    static FkColor &from(int32_t color);

    static FkColor &white();

    static FkColor &black();

public:
    FkColor();

    FkColor(const FkColor &o);

    virtual ~FkColor();

    float fRed();

    float fGreen();

    float fBlue();

    float fAlpha();

    float *fArray();

    int32_t toInt();

public:
    kFormat format;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t alpha; // 0 mean transparent, 255 mean no transparent.
    float *_fArray = nullptr;
};


#endif //FK_BASE_FKCOLOR_H
