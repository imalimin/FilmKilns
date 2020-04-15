/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_BINARYUTILS_H
#define HWVC_ANDROID_BINARYUTILS_H

#include "Object.h"

class BinaryUtils : public Object {
public:
    static bool match(const uint8_t *p, initializer_list<int8_t> args);
};


#endif //HWVC_ANDROID_BINARYUTILS_H
