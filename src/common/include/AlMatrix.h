//
// Created by mingyi.li on 2019-11-27.
//

#ifndef HWVC_ANDROID_ALMATRIX_H
#define HWVC_ANDROID_ALMATRIX_H

#include "Object.h"
#include "AlVec4.h"

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

    AlVec4 &operator*(AlVec4 v);

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
