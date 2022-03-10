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
#include "FkImageEngine.h"

const FkID FkImageFileEngine::FK_MSG_SAVE = 0x1;

static std::shared_ptr<FkImageEngine> _cast2ImageEngine(std::shared_ptr<FkEngine> &imageEngine) {
    return std::dynamic_pointer_cast<FkImageEngine>(imageEngine);
}

FkImageFileEngine::FkImageFileEngine(std::shared_ptr<FkEngine> &imageEngine, std::string name)
        : FkEngine(name), imageEngine(imageEngine) {
    FK_MARK_SUPER
    FK_REG_MSG(FK_MSG_SAVE, FkImageFileEngine::_onSave);
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

FkResult FkImageFileEngine::save(std::string file) {
    auto msg = FkMessage::obtain(FK_MSG_SAVE);
    msg->arg3 = std::move(file);
    return sendMessage(msg);
}

FkResult FkImageFileEngine::_onSave(std::shared_ptr<FkMessage> &msg) {
    return FK_OK;
}
