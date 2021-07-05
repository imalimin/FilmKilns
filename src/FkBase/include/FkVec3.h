/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKVEC3_H
#define FK_BASE_FKVEC3_H

#include "FkObject.h"

template<typename T>
FK_CLASS FkVec3 FK_EXTEND FkObject {
public:
    static void swap(FkVec3<T> *vec1, FkVec3<T> *vec2) {
        float tmp = vec1->x;
        vec1->x = vec2->x;
        vec2->x = tmp;

        tmp = vec1->y;
        vec1->y = vec2->y;
        vec2->y = tmp;
    };

public:
    FkVec3(T x, T y, T z) : FkObject(), x(x), y(y), z(z) {
        FK_MARK_SUPER
    };

    FkVec3(const FkVec3<T> &o) : FkObject(), x(o.x), y(o.y), z(o.z) {
        FK_MARK_SUPER
    };

    virtual ~FkVec3() {};

    virtual void set(FkVec3<T> vec) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    virtual FkVec3<T> operator+(const T value) {
        return FkVec3<T>(x + value, y + value, z + value);
    };

    virtual FkVec3<T> operator+(const FkVec3<T> &vec) {
        return FkVec3<T>(x + vec.x, y + vec.y, z + vec.z);
    };

    virtual FkVec3<T> operator-(const FkVec3<T> &vec) {
        return FkVec3<T>(x - vec.x, y - vec.y, z - vec.z);
    };

    virtual FkVec3<T> operator/(const T value) {
        return FkVec3<T>(x / value, y / value, z / value);
    }

    virtual FkVec3<T> operator/(const int32_t value) {
        return FkVec3<T>(x / value, y / value, z / value);
    }

    virtual bool operator<(const FkVec3<T> &vec) {
        return x < vec.x && y < vec.y && z < vec.z;
    };

    virtual bool operator>(const FkVec3<T> &vec) {
        return x > vec.x && y > vec.y && z > vec.z;
    };

    virtual bool operator==(const FkVec3<T> &vec) {
        return x == vec.x && y == vec.y && z == vec.z;
    };

    virtual bool isZero() {
        return 0 == x && 0 == y && 0 == z;
    };

public:
    T x;
    T y;
    T z;
};

#endif //FK_BASE_FKVEC3_H