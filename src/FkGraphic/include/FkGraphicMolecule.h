/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICMOLECULE_H
#define FK_GRAPHIC_FKGRAPHICMOLECULE_H

#include "FkMolecule.h"
#include "FkLocalClient.h"

FK_CLASS FkGraphicMolecule FK_EXTEND FkMolecule {
public:
    FkGraphicMolecule();

    FkGraphicMolecule(const FkGraphicMolecule &o) = delete;

    virtual ~FkGraphicMolecule();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onDrawLayer(std::shared_ptr<FkProtocol> p);

private:
    std::shared_ptr<FkSessionClient> client;
};


#endif //FK_GRAPHIC_FKGRAPHICMOLECULE_H
