/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALABSELEMENTED_H
#define HWVC_ANDROID_ALABSELEMENTED_H

#include "Object.h"
#include "AlElement.h"
#include "HwResult.h"

al_interface(AlAbsElemented) {
public:
    AlAbsElemented() : Object() {};

    ~AlAbsElemented() {};

    virtual HwResult fromElement(AlElement *element) = 0;

    virtual HwResult toElement(AlElement **element) = 0;

private:
    AlAbsElemented(const AlAbsElemented &e) : Object() {};
};

#endif //HWVC_ANDROID_ALABSELEMENTED_H
