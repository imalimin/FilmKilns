/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGELAYER_H
#define HWVC_ANDROID_ALIMAGELAYER_H

#include "Object.h"

al_class(AlImageLayer) {
public:
    AlImageLayer();

    ~AlImageLayer();

private:
    AlImageLayer(const AlImageLayer &e) : Object() {};
};


#endif //HWVC_ANDROID_ALIMAGELAYER_H
