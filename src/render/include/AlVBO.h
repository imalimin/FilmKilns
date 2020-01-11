/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALVBO_H
#define HWVC_ANDROID_ALVBO_H

#include "Object.h"
#include "AlBuffer.h"

al_class(AlVBO) {
public:
    static AlVBO *alloc();

public:
    virtual ~AlVBO();

    void bind();

    void unbind();

    void update(AlBuffer *buf);

private:
    AlVBO();

    AlVBO(const AlVBO &o) : Object() {};
private:
    uint32_t o = 0;
    bool allocated = false;
};


#endif //HWVC_ANDROID_ALVBO_H
