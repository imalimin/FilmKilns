/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKRENDERMOLECULE_H
#define FK_GRAPHIC_FKRENDERMOLECULE_H

#include "FkSimpleMolecule.h"

FK_CLASS FkRenderMolecule FK_EXTEND FkSimpleMolecule {
public:
    FkRenderMolecule();

    FkRenderMolecule(const FkRenderMolecule &o) = delete;

    virtual ~FkRenderMolecule();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual std::shared_ptr<FkQuarkContext> shareContextToSubQuark() override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    virtual void onConnect(std::shared_ptr<FkConnectChain> chain) override;

private:
    std::shared_ptr<FkQuarkContext> context = nullptr;
};

#endif //FK_GRAPHIC_FKRENDERMOLECULE_H