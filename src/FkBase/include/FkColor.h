/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKCOLOR_H
#define FK_BASE_FKCOLOR_H

#include "FkObject.h"
#include <vector>

FK_SUPER_CLASS(FkColor, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkColor)

public:
    FK_ENUM kFormat : uint32_t {
        NONE = 0,
        LUMINANCE,
        LUMINANCE2,
        RGB,
        RGBA,
        RGB565,
        YV12,
        NV12,
        END
    };
    FK_ENUM AlphaType : uint32_t {
        kUnPreMultiple = 0,
        kPreMultiple,
    };
public:
    static FkColor makeFrom(int32_t color);

    static FkColor makeFromRGBA8(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    static FkColor white();

    static FkColor black();

    static FkColor red();

    static FkColor green();

    static FkColor blue();

    static FkColor yellow();

    static FkColor transparent();

public:
    FkColor();

    FkColor(const FkColor &o);

    virtual ~FkColor();

    float fRed() const;

    float fGreen() const;

    float fBlue() const;

    float fAlpha() const;

    uint16_t uRed() const;

    uint16_t uGreen() const;

    uint16_t uBlue() const;

    uint16_t uAlpha() const;

    float *fArray() const;

    int32_t toInt() const;

    void setAlphaType(AlphaType type);

    bool isPreMultiple() const;

    kFormat getFormat() const;

    bool equals(FkColor &color);

    bool operator==(FkColor &color);

private:
    kFormat format;
    AlphaType alphaType;
    std::vector<uint16_t> vec; // RGBA16.Alpha 0 mean transparent, 255 mean no transparent.
    float *_fArray = new float[4];
};


#endif //FK_BASE_FKCOLOR_H
