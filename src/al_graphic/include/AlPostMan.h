/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPOSTMAN_H
#define HWVC_ANDROID_ALPOSTMAN_H

#include "UnitPipeline.h"

class AlAbsPoster : Object {
public:
    virtual void postMessage(AlMessage *msg) = 0;
};

class AlPostMan : public UnitPipeline, AlAbsPoster {
public:
    static AlPostMan *create(const string &name);

public:
    virtual ~AlPostMan();

    virtual int registerAnUnit(Unit *unit) override;

private:
    AlPostMan(const string &name);

    AlPostMan(const AlPostMan &o) : UnitPipeline("") {};

    void postMessage(AlMessage *msg) override;
};


#endif //HWVC_ANDROID_ALPOSTMAN_H
