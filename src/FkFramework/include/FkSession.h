/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKSESSION_H
#define FK_FRAMEWORK_FKSESSION_H

#include "FkObject.h"
#include "FkProtocol.h"
#include "FkQuark.h"
#include "FkEngineMonitor.h"
#include <vector>

//#define FK_CONNECT_TO(session, quark) (session->connectTo(std::static_pointer_cast<FkQuark>(quark)))
//#define FK_DISCONNECT_TO(session, quark) (session->disconnect(std::static_pointer_cast<FkQuark>(quark)))

class FkSessionClient;
class FkLocalClient;
class FkRemoteClient;

FK_SUPER_CLASS(FkSession, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkSession)
    friend FkSessionClient;
    friend FkLocalClient;
    friend FkRemoteClient;
public:
    FK_ENUM kState : int {
        IDL = -1,
        OPENED,
    };

    static std::shared_ptr<FkSession> with(FkProtocol::Desc protoDesc);

public:
    FkSession();

    virtual ~FkSession();

    FkResult connectTo(const std::shared_ptr<FkQuark> quark);

    FkResult disconnect(const std::shared_ptr<FkQuark> quark);

    FkResult open();

    FkResult close();

    virtual std::string toString() override;

private:
    FkSession(const FkSession &o) : FkObject() {};

    FkResult send(std::shared_ptr<FkProtocol> protocol, std::shared_ptr<FkAbsEngineMonitor> monitor);

private:
    FkProtocol::Desc protoDesc = {0, 0};
    std::vector<std::shared_ptr<FkQuark>> link;
    std::mutex mtx;
    kState state = kState::IDL;
};


#endif //FK_FRAMEWORK_FKSESSION_H
