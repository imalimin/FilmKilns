/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPOSTMAN_H
#define HWVC_ANDROID_ALPOSTMAN_H

#include "UnitPipeline.h"

al_class_ex(AlPostMan, UnitPipeline) {
public:
    static AlPostMan *create(const string &name);

public:
    virtual ~AlPostMan();

private:
    AlPostMan(const string &name);

    AlPostMan(const AlPostMan &o) : UnitPipeline("") {};
};


#endif //HWVC_ANDROID_ALPOSTMAN_H
