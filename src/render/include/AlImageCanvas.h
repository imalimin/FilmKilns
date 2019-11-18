/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGECANVAS_H
#define HWVC_ANDROID_ALIMAGECANVAS_H

#include "Object.h"

al_class(AlImageCanvas) {
public:
    AlImageCanvas();

    ~AlImageCanvas();

private:
    AlImageCanvas(const AlImageCanvas &e) : Object() {};
};


#endif //HWVC_ANDROID_ALIMAGECANVAS_H
