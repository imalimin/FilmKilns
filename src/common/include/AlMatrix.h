//
// Created by mingyi.li on 2019-11-27.
//

#ifndef HWVC_ANDROID_ALMATRIX_H
#define HWVC_ANDROID_ALMATRIX_H

#include "Object.h"

al_class(AlMatrix) {
public:
    static AlMatrix &fromArray(float *array);

    AlMatrix();

    AlMatrix(const AlMatrix &o);

    virtual ~AlMatrix();

    virtual void setScale(float scaleX, float scaleY);

    /**
     * 旋转
     * @param rotation 旋转弧度
     */
    virtual void setRotation(float rotation);

    virtual void setTranslate(float x, float y);

    AlMatrix &operator*(AlMatrix m);

    virtual float *data();

    void dump();

private:
    void _set(int32_t x, int32_t y, float val);

    float _get(int32_t x, int32_t y);

public:
    static const float PI;
    static const int SIZE;

private:
    float matrix[16] = {1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f};
};


#endif //HWVC_ANDROID_ALMATRIX_H
