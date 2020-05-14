/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALABSCONTEXT_H
#define HWVC_ANDROID_ALABSCONTEXT_H

#include "Object.h"

al_class AlAbsContext al_extend Object {
public:
    AlAbsContext();

    virtual ~AlAbsContext();

private:
    AlAbsContext(const AlAbsContext &o) : Object() {};
};


#endif //HWVC_ANDROID_ALABSCONTEXT_H
