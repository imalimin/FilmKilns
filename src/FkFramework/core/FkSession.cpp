/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSession.h"

FkSession::FkSession() : FkObject() {

}

FkSession::~FkSession() {

}

FkResult FkSession::connectTo(const std::shared_ptr<FkQuark> quark) {
    if (nullptr == quark) {
        return FK_FAIL;
    }
    if (FK_OK == quark->accept(templateProtocol)) {
        link.emplace_back(quark);
        return FK_OK;
    }
    return FK_FAIL;
}

void FkSession::close() {
    link.clear();
}