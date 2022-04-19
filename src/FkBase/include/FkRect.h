/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-19 13:35:25
*/

#ifndef FK_BASE_FKRECT_H
#define FK_BASE_FKRECT_H

#include "FkObject.h"

template<typename T>
FK_CLASS FkRect FK_EXTEND FkObject {
public:
    FkRect(T left, T top, T right, T bottom)
            : FkObject(), _left(left), _top(top), _right(right), _bottom(bottom) {
        FK_MARK_SUPER
    }

    FkRect(const FkRect &o)
            : FkObject(o), _left(o._left), _top(o._top), _right(o._right), _bottom(o._bottom) {
        FK_MARK_SUPER
    }

    virtual ~FkRect() {

    }

    T left() {
        return _left;
    }

    T top() {
        return _top;
    }

    T right() {
        return _right;
    }

    T bottom() {
        return _bottom;
    }

private:
    T _left = 0;
    T _top = 0;
    T _right = 0;
    T _bottom = 0;
};

FK_CLASS FkIntRect FK_EXTEND FkRect<int32_t> {
public:
    FkIntRect(int32_t left, int32_t top, int32_t right, int32_t bottom);

    FkIntRect(const FkIntRect &o);

    virtual ~FkIntRect();
};

FK_CLASS FkFloatRect FK_EXTEND FkRect<float> {
public:
    FkFloatRect(float left, float top, float right, float bottom);

    FkFloatRect(const FkFloatRect &o);

    virtual ~FkFloatRect();

};

#endif //FK_BASE_FKRECT_H