/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKHANDLER_H
#define FK_BASE_FKHANDLER_H

#include "FkObject.h"
#include "FkMessage.h"
#include "FkLooper.h"
#include "FkDefinition.h"

FK_CLASS FkHandler FK_EXTEND FkObject {
public:
    typedef std::function<void(std::shared_ptr<FkMessage> msg)> Callback;
public:
    FkHandler(Callback callback);

    FkHandler(FkLooper *looper, Callback callback);

    ~FkHandler();

    void sendMessage(std::shared_ptr<FkMessage> msg);

    void dispatchMessage(std::shared_ptr<FkMessage> msg);

private:
    void _enqueueMessage(std::shared_ptr<FkMessage> msg);

private:
    FkLooper *looper = nullptr;
    Callback callback = nullptr;

};


#endif //FK_BASE_FKHANDLER_H
