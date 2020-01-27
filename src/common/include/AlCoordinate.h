/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCOORDINATE_H
#define HWVC_ANDROID_ALCOORDINATE_H

#include "Object.h"
#include "AlVec2.h"
#include "AlMatrix.h"

al_class(AlCoordinate) {
public:
    static AlCoordinate &create();

    AlCoordinate(const AlCoordinate &o);

    ~AlCoordinate();

    void translate(int32_t &x, int32_t &y);

    void setScale(float scaleX, float scaleY);

    void setRotation(float alpha);

    void seTranslate(int32_t x, int32_t y);

private:
    AlCoordinate() : Object() {};

private:
    AlMatrix mat;
    int32_t transX = 0, transY = 0;
};


#endif //HWVC_ANDROID_ALCOORDINATE_H
