/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALFILEDESCRIPTOR_H
#define HWVC_ANDROID_ALFILEDESCRIPTOR_H

#include "AlAbsInputDescriptor.h"

AL_INTERFACE AlFileDescriptor AL_EXTEND AlAbsInputDescriptor {
public:
    AlFileDescriptor(std::string path);

    AlFileDescriptor(const AlFileDescriptor &o);

    virtual ~AlFileDescriptor();

private:
    std::string _path;
};


#endif //HWVC_ANDROID_ALFILEDESCRIPTOR_H
