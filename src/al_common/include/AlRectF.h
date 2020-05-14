/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALRECTF_H
#define HWVC_ANDROID_ALRECTF_H

#include "Object.h"

al_class AlRectF al_extend Object {
public:
    AlRectF();

    AlRectF(float left, float top, float right, float bottom);

    AlRectF(const AlRectF &o);

    virtual ~AlRectF();

    float getWidth();

    float getHeight();

    void set(float left, float top, float right, float bottom);

    void dump();

public:
    float left = 0.0f;
    float top = 0.0f;
    float right = 0.0f;
    float bottom = 0.0f;
};


#endif //HWVC_ANDROID_ALRECTF_H
