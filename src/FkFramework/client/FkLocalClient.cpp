/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkLocalClient.h"

FK_IMPL_CLASS_TYPE(FkLocalClient, FkSessionClient)

FkLocalClient::FkLocalClient() : FkSessionClient() {

}

FkLocalClient::~FkLocalClient() {

}

void FkLocalClient::setMonitor(std::shared_ptr<FkAbsEngineMonitor> &_monitor) {
    this->monitor = _monitor;
}

FkResult FkLocalClient::send(std::shared_ptr<FkSession> session,
                             std::shared_ptr<FkProtocol> protocol) {
    return session->send(protocol, this->monitor);
}
