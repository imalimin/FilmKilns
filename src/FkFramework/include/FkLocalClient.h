/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKREMOTECLIENT_H
#define FK_FRAMEWORK_FKREMOTECLIENT_H

#include "FkSessionClient.h"

FK_ABS_CLASS FkLocalClient FK_EXTEND FkSessionClient {
public:
    FkLocalClient(const FkLocalClient &o) = delete;

    FkLocalClient();

    virtual ~FkLocalClient();

    virtual FkResult send(std::shared_ptr<FkSession> session, std::shared_ptr<FkProtocol> protocol) override ;
};


#endif //FK_FRAMEWORK_FKREMOTECLIENT_H
