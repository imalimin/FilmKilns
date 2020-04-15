/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_AL2DCOORDINATE_H
#define HWVC_ANDROID_AL2DCOORDINATE_H

#include "Object.h"
#include "AlVec2.h"
#include "AlSize.h"
#include "AlRational.h"

al_class(Al2DCoordinate) {
public:
    Al2DCoordinate(int32_t xWide = 0, int32_t yWide = 0);

    Al2DCoordinate(const Al2DCoordinate &o);

    virtual ~Al2DCoordinate();

    void setWide(int32_t x, int32_t y);

    AlSize getRegion();

    void setScale(float scaleX, float scaleY);

    void setRotation(AlRational r);

    void setPosition(float x, float y);

    void translate(AlVec2 *vec, Al2DCoordinate *dstCoord);

private:
    AlVec2 wide;
    AlVec2 scale;
    AlRational rotation = AlRational();
    AlVec2 position;
};


#endif //HWVC_ANDROID_AL2DCOORDINATE_H
