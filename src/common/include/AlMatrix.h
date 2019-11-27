//
// Created by mingyi.li on 2019-11-27.
//

#ifndef HWVC_ANDROID_ALMATRIX_H
#define HWVC_ANDROID_ALMATRIX_H

#include "Object.h"

al_class(AlMatrix) {
public:
    AlMatrix();

    AlMatrix(const AlMatrix &o);

    virtual ~AlMatrix();

    void setScale(float scaleX, float scaleY);

    void setRotation(float rotation);

    float *data();

public:
    static const float PI;
    static const int SIZE;

private:
    float matrix[4][4] = {1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, 1.0f};
};


#endif //HWVC_ANDROID_ALMATRIX_H
