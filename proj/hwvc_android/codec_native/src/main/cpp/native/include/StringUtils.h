/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HARDWAREVIDEOCODEC_STRINGUTILS_H
#define HARDWAREVIDEOCODEC_STRINGUTILS_H

#include <jni.h>
#include <string>

using namespace std;
namespace StringUtils {
    char **jStringArray2StringArray(JNIEnv *env, jobjectArray jStringArray, int &size);

    void releaseStringArray(char **array, int size);
}

#endif //HARDWAREVIDEOCODEC_STRINGUTILS_H
