/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-06-20 21:24:53
*/

#ifndef FK_FRAMEWORK_FKENGINEMONITOR_H
#define FK_FRAMEWORK_FKENGINEMONITOR_H

#include "FkObject.h"

FK_SUPER_CLASS(FkEngineMonitor, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkEngineMonitor)

public:
    FkEngineMonitor();

    FkEngineMonitor(const FkEngineMonitor &o);

    virtual ~FkEngineMonitor();
};

#endif //FK_FRAMEWORK_FKENGINEMONITOR_H