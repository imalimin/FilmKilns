/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALABSINPUTDESCRIPTOR_H
#define HWVC_ANDROID_ALABSINPUTDESCRIPTOR_H

#include "Object.h"

AL_CLASS AlAbsInputDescriptor AL_EXTEND Object {
public:
    AL_ENUM kType : int32_t {
        UNKNOWN = -1,
        FILE,
    };
public:
    AlAbsInputDescriptor(kType type);

    AlAbsInputDescriptor(const AlAbsInputDescriptor &o);

    virtual ~AlAbsInputDescriptor();

    kType type();

private:
    kType _type = kType::UNKNOWN;
};


#endif //HWVC_ANDROID_ALABSINPUTDESCRIPTOR_H
