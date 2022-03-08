/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-8 12:58:51
*/

#include "FkImageFileEngine.h"

FkImageFileEngine::FkImageFileEngine(std::shared_ptr<FkEngine> &imageEngine, std::string name) : FkEngine(name) {
    FK_MARK_SUPER
}

FkImageFileEngine::~FkImageFileEngine() {

}

FkResult FkImageFileEngine::onCreate() {
    auto ret = FkEngine::onCreate();
    return ret;
}

FkResult FkImageFileEngine::onDestroy() {
    auto ret = FkEngine::onDestroy();
    return ret;
}

FkResult FkImageFileEngine::onStart() {
    auto ret = FkEngine::onStart();
    return ret;
}

FkResult FkImageFileEngine::onStop() {
    auto ret = FkEngine::onStop();
    return ret;
}