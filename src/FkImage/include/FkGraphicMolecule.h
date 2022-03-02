/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICMOLECULE_H
#define FK_GRAPHIC_FKGRAPHICMOLECULE_H

#include "FkSimpleMolecule.h"
#include "FkAtom.h"
#include "FkLocalClient.h"
#include "FkRenderEngine.h"

FK_CLASS FkGraphicMolecule FK_EXTEND FkSimpleMolecule {
public:
    FkGraphicMolecule();

    FkGraphicMolecule(const FkGraphicMolecule &o) = delete;

    virtual ~FkGraphicMolecule();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual std::shared_ptr<FkQuarkContext> shareContextToSubQuark() override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    virtual void onConnect(std::shared_ptr<FkConnectChain> chain) override;

private:
    FkResult _onDrawLayer(std::shared_ptr<FkProtocol> p);

private:
    std::shared_ptr<FkRenderEngine> renderEngine = nullptr;
    std::shared_ptr<FkQuarkContext> context = nullptr;
};


#endif //FK_GRAPHIC_FKGRAPHICMOLECULE_H
