/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_ANDROID_TEST_FKSPOBJECT_H
#define FK_ANDROID_TEST_FKSPOBJECT_H

#include "SkRefCnt.h"
class FkSPObject2;
class FkSPObject : public SkRefCnt {
public:
    FkSPObject();
    virtual ~FkSPObject();
    sk_sp<FkSPObject2> obj = nullptr;
};

class FkSPObject2 : public SkRefCnt {
public:
    FkSPObject2();
    virtual ~FkSPObject2();
    sk_sp<FkSPObject> obj = nullptr;
};

#endif //FK_ANDROID_TEST_FKSPOBJECT_H
