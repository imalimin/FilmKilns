/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKRENDERPROCESSATOM_H
#define FK_GRAPHIC_FKRENDERPROCESSATOM_H

#include "FkSimpleMolecule.h"

FK_CLASS FkRenderProcessAtom FK_EXTEND FkSimpleMolecule {
public:
    FkRenderProcessAtom();

    FkRenderProcessAtom(const FkRenderProcessAtom &o) = delete;

    virtual ~FkRenderProcessAtom();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    virtual void onConnect(std::shared_ptr<FkConnectChain> chain) override;
};

#endif //FK_GRAPHIC_FKRENDERPROCESSATOM_H