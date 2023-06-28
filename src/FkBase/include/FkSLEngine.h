/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKSLENGINE_H
#define FK_BASE_FKSLENGINE_H

#include "FkObject.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

FK_SUPER_CLASS(FkSLEngine, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkSLEngine)

public:
    FkSLEngine();

    ~FkSLEngine();

    virtual bool valid();

    virtual SLEngineItf getEngine() {
        return engineItf;
    }

private:
    SLObjectItf engineObject = nullptr;
    SLEngineItf engineItf = nullptr;

    void release();

};


#endif //FK_BASE_FKSLENGINE_H
