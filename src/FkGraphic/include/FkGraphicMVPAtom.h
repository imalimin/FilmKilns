/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICMVPATOM_H
#define FK_GRAPHIC_FKGRAPHICMVPATOM_H

#include "FkAtom.h"
#include "FkLocalClient.h"

FK_CLASS FkGraphicMVPAtom FK_EXTEND FkAtom {
public:
    FkGraphicMVPAtom();

    FkGraphicMVPAtom(const FkGraphicMVPAtom &o) = delete;

    virtual ~FkGraphicMVPAtom();

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
    std::shared_ptr<FkSession> mLayerSession = nullptr;
    std::shared_ptr<FkQuark> mTransQuark = nullptr;
    std::shared_ptr<FkQuark> mScaleQuark = nullptr;
    std::shared_ptr<FkQuark> mRotateQuark = nullptr;
};


#endif //FK_GRAPHIC_FKGRAPHICMVPATOM_H
