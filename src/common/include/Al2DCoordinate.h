//
// Created by mingyi.li on 2020-03-22.
//

#ifndef HWVC_ANDROID_AL2DCOORDINATE_H
#define HWVC_ANDROID_AL2DCOORDINATE_H

#include "Object.h"
#include "AlVec2.h"
#include "AlRational.h"

al_class(Al2DCoordinate) {
public:
    Al2DCoordinate(int32_t xWide = 0, int32_t yWide = 0);

    Al2DCoordinate(const Al2DCoordinate &o);

    virtual ~Al2DCoordinate();

    void setWide(int32_t x, int32_t y);

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
