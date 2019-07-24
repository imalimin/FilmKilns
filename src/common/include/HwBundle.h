/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWBUNDLE_H
#define HWVC_ANDROID_HWBUNDLE_H

#include "Object.h"
#include <map>

using namespace std;

class HwBundle : public Object {
public:
    HwBundle();

    virtual ~HwBundle();

    void putInt32(string key, int32_t value);

    void putInt64(string key, int64_t value);

    int32_t getInt32(string key);

    int64_t getInt64(string key);

private:
    map<string, int32_t> int32Map;
    map<string, int64_t> int64Map;
};


#endif //HWVC_ANDROID_HWBUNDLE_H
