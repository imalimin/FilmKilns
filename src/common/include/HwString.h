/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWSTRING_H
#define HWVC_ANDROID_HWSTRING_H

#include "Object.h"
#include <string>

using namespace std;

class HwString : public Object {
public:
    HwString(string str);

    HwString(const HwString &str);

    virtual ~HwString();

    string str();

private:
    string value;
};


#endif //HWVC_ANDROID_HWSTRING_H
