/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICSOURCEATOM_H
#define FK_GRAPHIC_FKGRAPHICSOURCEATOM_H

#include "FkSimpleAtom.h"

FK_SUPER_CLASS(FkGraphicSourceAtom, FkSimpleAtom) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicSourceAtom)

public:
    FkGraphicSourceAtom();

    FkGraphicSourceAtom(const FkGraphicSourceAtom &o) = delete;

    virtual ~FkGraphicSourceAtom();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    virtual void onConnect(std::shared_ptr<FkConnectChain> chain) override;

private:
    FkResult _onSetSurface(std::shared_ptr<FkProtocol> p);

};


#endif //FK_GRAPHIC_FKGRAPHICSOURCEATOM_H
