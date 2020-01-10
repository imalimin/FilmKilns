/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCOLOR_H
#define HWVC_ANDROID_ALCOLOR_H

#include "Object.h"

al_class(AlColor) {
public:
    static AlColor from(std::string colorStr);

public:
    AlColor(int32_t argb);

    AlColor(const AlColor &o);

    ~AlColor();

    int32_t color();

    uint8_t r();

    uint8_t g();

    uint8_t b();

    uint8_t a();

    float rf();

    float gf();

    float bf();

    float af();

private:
    int32_t _argb = 0;

};


#endif //HWVC_ANDROID_ALCOLOR_H
