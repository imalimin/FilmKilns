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

FK_SUPER_TEMPLATE_CLASS(FkNumber, FkObject, typename T) {
FK_DEF_CLASS_TYPE_FUNC(FkNumber)

public:
    FkNumber(T value) : FkObject(), value(value) {  }

    FkNumber(const FkNumber &o) : FkObject(), value(o.value) {  }

    virtual ~FkNumber() {}

    T get() {
        return value;
    }

private:
    T value;
};

FK_SUPER_TEMPLATE_CLASS_IMPL(FkInt, FkNumber)<int32_t> {
FK_DEF_CLASS_TYPE_FUNC(FkInt)

public:
    FkInt(int32_t value) : FkNumber<int32_t>(value) {  }

    FkInt(const FkInt &o) : FkNumber<int32_t>(o) {  }

    virtual ~FkInt() {}
};

#endif //FKANDROID_FKNUMBER_H
