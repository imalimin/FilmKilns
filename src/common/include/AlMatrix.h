/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMATRIX_H
#define HWVC_ANDROID_ALMATRIX_H

#include "Object.h"
#include "AlVec4.h"

class AlVec4;

/// [4x4] matrix
al_class(AlMatrix) {
public:
    static AlMatrix &fromArray(float *array);

    AlMatrix();

    AlMatrix(const AlMatrix &o);

    virtual ~AlMatrix();

    virtual void setScale(float scaleX, float scaleY);

    virtual void setRotation(float rotation);

    virtual void setTranslate(float x, float y);

    /// 求转置矩阵
    /// \return 转置矩阵
    AlMatrix &T();

    AlMatrix &operator*(AlMatrix m);

    /// [4x4] x [4x1]
    /// \param v
    /// \return
//    AlVec4 &operator*(AlVec4 v);

    /// 根据下标取矩阵的值
    /// \param row 行
    /// \param col 列
    /// \return 下标对应的值
    float get(int32_t row, int32_t col);

    virtual float *data();

    void dump();

private:
    void _set(int32_t row, int32_t col, float val);

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
