/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALSIZE_H
#define HWVC_ANDROID_ALSIZE_H

#include "Size.h"

al_class AlSize al_extend Size {
public:
    AlSize();

    AlSize(const AlSize &o);

    AlSize(int w, int h);

    virtual ~AlSize();

    AlSize &operator=(const AlSize &o);

    bool operator==(const AlSize &o);

    bool operator!=(const AlSize &o);

};


#endif //HWVC_ANDROID_ALSIZE_H
