/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKMOLECULE_H
#define FK_FRAMEWORK_FKMOLECULE_H

#include "FkAtom.h"

FK_SUPER_CLASS(FkMolecule, FkAtom) {
FK_DEF_CLASS_TYPE_FUNC(FkMolecule)

public:
    FkMolecule();

    FkMolecule(const FkMolecule &o) = delete;

    virtual ~FkMolecule();

protected:
    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

};


#endif //FK_FRAMEWORK_FKMOLECULE_H
