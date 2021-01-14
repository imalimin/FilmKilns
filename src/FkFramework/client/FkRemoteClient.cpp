/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRemoteClient.h"

FkRemoteClient::FkRemoteClient() : FkSessionClient() {

}

FkRemoteClient::~FkRemoteClient() {

}

FkResult FkRemoteClient::send(std::shared_ptr<FkSession> session,
                              std::shared_ptr<FkProtocol> protocol) {
    return 0;
}