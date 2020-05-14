/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALIOUTILS_H
#define HWVC_ANDROID_ALIOUTILS_H

#include "Object.h"
#include "AlBuffer.h"
#include "HwResult.h"

al_class AlIOUtils al_extend Object {
public:
    static HwResult readFile(std::string *inFile, AlBuffer **outBuf);

private:
    AlIOUtils() : Object() {};

    ~AlIOUtils() {};

    AlIOUtils(const AlIOUtils &o) : Object() {}

};


#endif //HWVC_ANDROID_ALIOUTILS_H
