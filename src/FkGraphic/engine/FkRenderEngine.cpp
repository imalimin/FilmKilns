/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderEngine.h"

FkRenderEngine::FkRenderEngine(std::string name) : FkEngine(name) {
    FK_MARK_SUPER
}

FkRenderEngine::~FkRenderEngine() {

}
FkResult FkRenderEngine::onCreate() {
    auto ret = FkEngine::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnCreatePrt>(molecule);
    return ret;
}

FkResult FkRenderEngine::onDestroy() {
    auto ret = FkEngine::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnDestroyPrt>(molecule);
    return ret;
}

FkResult FkRenderEngine::onStart() {
    auto ret = FkEngine::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnStartPrt>(molecule);
    return ret;
}

FkResult FkRenderEngine::onStop() {
    auto ret = FkEngine::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    client->quickSend<FkOnStopPrt>(molecule);
    return ret;
}