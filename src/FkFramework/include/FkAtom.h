/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKATOM_H
#define FK_FRAMEWORK_FKATOM_H

#include "FkQuark.h"
#include "FkSession.h"
#include "FkSessionClient.h"
#include "FkOnCreatePrt.h"
#include "FkOnDestroyPrt.h"
#include "FkOnStartPrt.h"
#include "FkOnStopPrt.h"

FK_SUPER_CLASS(FkAtom, FkQuark) {
FK_DEF_CLASS_TYPE_FUNC(FkAtom)

public:
    FkAtom();

    FkAtom(const FkAtom &o) = delete;

    virtual ~FkAtom();

protected:
    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

};


#endif //FK_FRAMEWORK_FKATOM_H
