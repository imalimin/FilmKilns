/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWFBOBJECT_H
#define HWVC_ANDROID_HWFBOBJECT_H

#include "HwAbsFBObject.h"

class HwFBObject : public HwAbsFBObject {
public:
    static HwAbsFBObject *alloc();

public:
    virtual ~HwFBObject();

private:
    HwFBObject();
};


#endif //HWVC_ANDROID_HWFBOBJECT_H
