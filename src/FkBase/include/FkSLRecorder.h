/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/
#ifndef FK_BASE_FKSLRECORDER_H
#define FK_BASE_FKSLRECORDER_H

#include "FkObject.h"
#include "FkSLEngine.h"
#include "FkAudioDevice.h"

FK_SUPER_CLASS(FkSLRecorder, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkSLRecorder)

public:
    FkSLRecorder(FkSLEngine *engine);

    ~FkSLRecorder();

    FkResult initialize(FkSLAudioDevice *device);

    FkResult registerCallback(slAndroidSimpleBufferQueueCallback callback,
                              void *pContext);

    FkResult start();

    FkResult stop();

    FkResult clear();

    SLAndroidSimpleBufferQueueItf getQueue();

private:
    void release();

private:
    FkSLEngine *engine = nullptr;
    SLObjectItf recordObject = nullptr;
    SLRecordItf recordItf = nullptr;
    SLAndroidSimpleBufferQueueItf bufferQueueItf = nullptr;

};


#endif //FK_BASE_FKSLRECORDER_H
