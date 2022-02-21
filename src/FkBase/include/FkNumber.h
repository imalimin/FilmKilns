/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-21 22:28:39
*/

#ifndef FKANDROID_FKNUMBER_H
#define FKANDROID_FKNUMBER_H

#include "FkObject.h"

template<typename T>
FK_ABS_CLASS FkNumber FK_EXTEND FkObject {
public:
    FkNumber(T value) : FkObject(), value(value) { FK_MARK_SUPER }

    FkNumber(const FkNumber &o) : FkObject(), value(o.value) { FK_MARK_SUPER }

    virtual ~FkNumber() {}

    T get() {
        return value;
    }

private:
    T value;
};

FK_ABS_CLASS FkInt FK_EXTEND FkNumber<int32_t> {
public:
    FkInt(int32_t value) : FkNumber<int32_t>(value) { FK_MARK_SUPER }

    FkInt(const FkInt &o) : FkNumber<int32_t>(o) { FK_MARK_SUPER }

    virtual ~FkInt() {}
};

#endif //FKANDROID_FKNUMBER_H
