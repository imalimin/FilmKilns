/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKSIMPLEATOM_H
#define FK_FRAMEWORK_FKSIMPLEATOM_H

#include "FkAtom.h"
#include "FkLocalClient.h"
#include <list>
#include <map>

FK_ABS_CLASS FkConnectChain FK_EXTEND FkObject {
public:
    FkConnectChain();

    FkConnectChain(const FkConnectChain &o) = delete;

    virtual ~FkConnectChain();

    void next(std::shared_ptr<FkQuark> quark);

    std::shared_ptr<FkSession> connectSession(std::shared_ptr<FkProtocol> p);

private:
    std::list<std::shared_ptr<FkQuark>> chain;
};

FK_ABS_CLASS FkSimpleAtom FK_EXTEND FkAtom {
public:
    FkSimpleAtom();

    FkSimpleAtom(const FkSimpleAtom &o) = delete;

    virtual ~FkSimpleAtom();

protected:
    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    virtual FkResult dispatchNext(std::shared_ptr<FkProtocol> p);

    virtual void onConnect(std::shared_ptr<FkConnectChain> chain) = 0;

private:
    void _connectSession();

    void _disconnectSession();

private:
    std::shared_ptr<FkSessionClient> client;
    std::shared_ptr<FkConnectChain> chain;
    std::map<FkProtocol::Type, std::shared_ptr<FkSession>> mSessionMap;
};


#endif //FK_FRAMEWORK_FKSIMPLEATOM_H
