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
#include <vector>

class AlVec4;

/// [4x4] matrix
al_class(AlMatrix) {
public:
    static AlMatrix &fromArray(float *array);

    AlMatrix();

    AlMatrix(const AlMatrix &o);

    virtual ~AlMatrix();

    virtual void reset();

    virtual void setScale(float scaleX, float scaleY);

    virtual void setRotation(float rotation);

    virtual void setTranslate(float x, float y);

    /// 求转置矩阵
    /// \return 转置矩阵
    AlMatrix &T();

    AlMatrix &operator*(const AlMatrix &m);

    AlVec4 &operator*(const AlVec4 &v);

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
    static constexpr int SIZE = 18;

private:
    std::vector<float> matrix;
};


#endif //HWVC_ANDROID_ALMATRIX_H
