//
// Created by mingyi.li on 2019/7/2.
//

#ifndef HWVC_ANDROID_HWABSLIBLOADER_H
#define HWVC_ANDROID_HWABSLIBLOADER_H

#include "Object.h"
#include <string>
#include <dlfcn.h>

using namespace std;

class HwAbsLibLoader : public Object {
protected:
    HwAbsLibLoader();

    virtual ~HwAbsLibLoader();

    virtual bool open() = 0;

    virtual void load() = 0;

    virtual bool close() = 0;
};


#endif //HWVC_ANDROID_HWABSLIBLOADER_H
