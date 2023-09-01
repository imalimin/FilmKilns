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

FK_SUPER_TEMPLATE_CLASS(FkRect, FkObject, typename T) {
FK_DEF_CLASS_TYPE_FUNC(FkRect)

public:
    FkRect(T left, T top, T right, T bottom)
            : FkObject(), _left(left), _top(top), _right(right), _bottom(bottom) {

    }

    FkRect(const FkRect &o)
            : FkObject(o), _left(o._left), _top(o._top), _right(o._right), _bottom(o._bottom) {

    }

    virtual ~FkRect() {

    }

    T left() const {
        return _left;
    }

    T top() const {
        return _top;
    }

    T right() const {
        return _right;
    }

    T bottom() const {
        return _bottom;
    }

    T width() const {
        return _right - _left;
    }

    T height() const {
        return _bottom - _top;
    }

private:
    T _left = 0;
    T _top = 0;
    T _right = 0;
    T _bottom = 0;
};

FK_SUPER_TEMPLATE_CLASS_IMPL(FkIntRect, FkRect)<int32_t> {
FK_DEF_CLASS_TYPE_FUNC(FkIntRect)

public:
    FkIntRect(int32_t left, int32_t top, int32_t right, int32_t bottom);

    FkIntRect(const FkIntRect &o);

    virtual ~FkIntRect();
};

FK_SUPER_TEMPLATE_CLASS_IMPL(FkFloatRect, FkRect)<float> {
FK_DEF_CLASS_TYPE_FUNC(FkFloatRect)

public:
    FkFloatRect(float left, float top, float right, float bottom);

    FkFloatRect(const FkFloatRect &o);

    virtual ~FkFloatRect();

};

#endif //FK_BASE_FKRECT_H