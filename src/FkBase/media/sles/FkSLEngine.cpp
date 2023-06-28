/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSLEngine.h"

#define TAG "FkSLEngine"

FK_IMPL_CLASS_TYPE(FkSLEngine, FkObject)

FkSLEngine::FkSLEngine() : FkObject() {
    SLresult result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if (SL_RESULT_SUCCESS != result) {
        FkLogE(TAG, "slCreateEngine failed!");
        release();
        return;
    }
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        FkLogE(TAG, "Engine Realize failed!");
        release();
        return;
    }
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineItf);
    if (SL_RESULT_SUCCESS != result) {
        FkLogE(TAG, "Engine GetInterface failed!");
        release();
        return;
    }
}

FkSLEngine::~FkSLEngine() {
    release();
}

bool FkSLEngine::valid() {
    return nullptr != engineObject;
}

void FkSLEngine::release() {
    if (engineObject) {
        (*engineObject)->Destroy(engineObject);
        engineObject = nullptr;
        engineItf = nullptr;
    }
}
