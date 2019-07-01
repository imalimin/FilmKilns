/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwAndroidUtils.h"
#include "Logcat.h"
#include <sys/system_properties.h>
#include <cstdlib>

int HwAndroidUtils::getAndroidApi() {
    string key = "ro.build.version.sdk";
    char value[128] = {0};
    int ret = __system_property_get(key.c_str(), value);
    if (ret <= 0) {
        Logcat::e("HWVC", "HwJavaNativeHelper::getAndroidApi failed.");
        return 0;
    }
    return atoi(value);
}
