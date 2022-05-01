/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-1 13:20:29
*/

#ifndef FK_GRAPHIC_FKGLENVATOM_H
#define FK_GRAPHIC_FKGLENVATOM_H

#include "FkSimpleAtom.h"
#include "FkContextCompo.h"

FK_SUPER_CLASS(FkGLEnvAtom, FkSimpleAtom) {
FK_DEF_CLASS_TYPE_FUNC(FkGLEnvAtom)

public:
    FkGLEnvAtom();

    virtual ~FkGLEnvAtom();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    virtual void onConnect(std::shared_ptr<FkConnectChain> chain) override;

private:
    FkResult _onRender(std::shared_ptr<FkProtocol> p);

    FkResult _onUpdateWindow(std::shared_ptr<FkProtocol> p);

private:
    FkResult _changeWithWindow(std::shared_ptr<FkGraphicWindow> &win);

    FkResult _changeWithoutWindow();

    FkResult _initializeWithWindow(std::shared_ptr<FkGraphicWindow> &win);

    FkResult _initializeWithoutWindow();

    FkResult _destroyContext();

    bool _isOffWin(std::shared_ptr<FkGraphicWindow> &win);

private:
    std::shared_ptr<FkContextCompo> context = nullptr;
};

#endif //FK_GRAPHIC_FKGLENVATOM_H