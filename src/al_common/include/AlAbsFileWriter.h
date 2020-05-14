/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALABSFILEWRITER_H
#define HWVC_ANDROID_ALABSFILEWRITER_H

#include "Object.h"

al_class AlAbsFileWriter al_extend Object {
public:
    AlAbsFileWriter(std::string path);

    virtual ~AlAbsFileWriter();

    virtual void close() = 0;

private:
    std::string path;
};


#endif //HWVC_ANDROID_ALABSFILEWRITER_H
