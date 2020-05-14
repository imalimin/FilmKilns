/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALRECTLOC_H
#define HWVC_ANDROID_ALRECTLOC_H

#include "Object.h"

al_class AlRectLoc al_extend Object {
public:
    static AlRectLoc value(int32_t val);

    static AlRectLoc CENTER();

    static AlRectLoc LT();

    static AlRectLoc RT();

    static AlRectLoc RB();

    static AlRectLoc LB();

    static AlRectLoc LEFT();

    static AlRectLoc TOP();

    static AlRectLoc RIGHT();

    static AlRectLoc BOTTOM();

public:
    AlRectLoc(const AlRectLoc &o);

    virtual  ~AlRectLoc();

    bool operator==(const AlRectLoc &o);

    AlRectLoc operator|(const AlRectLoc &o);

    AlRectLoc operator&(const AlRectLoc &o);

    int32_t int32();

private:
    AlRectLoc(int32_t loc);

private:
    int32_t loc = 0;
};


#endif //HWVC_ANDROID_ALRECTLOC_H
