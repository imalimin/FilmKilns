/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSessionClient.h"

FK_IMPL_CLASS_TYPE(FkSessionBuilder, FkObject)
FK_IMPL_CLASS_TYPE(FkSessionClient, FkObject)

FkResult FkSessionBuilder::send(std::shared_ptr<FkProtocol> proto) {
    auto session = FkSession::with(proto);
    return client->send(session, proto, chain);
}

std::shared_ptr<FkSessionBuilder>
FkSessionBuilder::with(std::list<std::shared_ptr<FkQuark>> &quarks) {
    for (auto &it : quarks) {
        chain.emplace_back(it);
    }
    return shared_from_this();
}

FkSessionClient::FkSessionClient() : FkObject() {

}

FkSessionClient::~FkSessionClient() {

}
