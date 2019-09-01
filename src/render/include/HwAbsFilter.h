/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWABSFILTER_H
#define HWVC_ANDROID_HWABSFILTER_H

#include "Object.h"
#include "HwProgram.h"
#include "HwAbsTexture.h"
#include "HwAbsFBObject.h"

const int Hw_FILTER_BASE = 100;
const int Hw_FILTER_NONE = 2333;

class HwAbsFilter : public Object {
public:
    string name;

    HwAbsFilter();

    virtual ~HwAbsFilter();

    virtual bool init();

    virtual void draw(HwAbsTexture *src, HwAbsTexture *dest);

    virtual void setParams(int *params);

    virtual void setParam(int key, int value);

protected:
    HwProgram *program = nullptr;

private:
    HwAbsFBObject *fbo = nullptr;
    bool initialized = false;
};


#endif //HWVC_ANDROID_HWABSFILTER_H
