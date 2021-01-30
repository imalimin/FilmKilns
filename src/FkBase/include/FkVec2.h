/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKVEC2_H
#define FK_BASE_FKVEC2_H

#include "FkObject.h"

/**
 * position or scale params
 */
/// [x,y]
FK_CLASS FkVec2 FK_EXTEND FkObject {
public:
    static void swap(FkVec2 *vec1, FkVec2 *vec2);

public:
    FkVec2(float x = 0.0f, float y = 0.0f);

    FkVec2(const FkVec2 &o);

    virtual ~FkVec2();

    virtual void set(FkVec2 vec);

    virtual bool isZero();

    virtual FkVec2 &operator+(const float value);

    virtual FkVec2 &operator+(const FkVec2 &vec);

    virtual FkVec2 &operator-(const FkVec2 &vec);

    virtual bool operator<(const FkVec2 &vec);

    virtual bool operator>(const FkVec2 &vec);

    virtual bool operator==(const FkVec2 &vec);

    virtual FkVec2 &operator/(const float value);

    virtual bool isNan();

public:
    float x = 0.0f;
    float y = 0.0f;
};


#endif //FK_BASE_FKVEC2_H
