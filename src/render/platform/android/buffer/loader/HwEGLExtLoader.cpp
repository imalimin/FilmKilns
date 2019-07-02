/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwEGLExtLoader.h"
#include "Logcat.h"

HwEGLExtLoader *HwEGLExtLoader::getInstance() {
    static HwEGLExtLoader instance;
    return &instance;
}

HwEGLExtLoader::HwEGLExtLoader() : HwAbsLibLoader() {
    if (open()) {
        load();
    }
    close();
}

HwEGLExtLoader::~HwEGLExtLoader() {
}

bool HwEGLExtLoader::open() {
    if (pLibHandler) {
        return true;
    }
    pLibHandler = dlopen(LIB_NAME.c_str(), 0);
    if (!pLibHandler) {
        Logcat::e("HWVC", "HwEGLExtLoader::open %s failed.", LIB_NAME.c_str());
        return false;
    }
    return true;
}

void HwEGLExtLoader::load() {
    FeglGetNativeClientBufferANDROID = load_func<signature_B_PB>(pLibHandler,
                                                                 "eglGetNativeClientBufferANDROID");
}

bool HwEGLExtLoader::close() {
    if (pLibHandler) {
        dlclose(pLibHandler);
        pLibHandler = nullptr;
        return true;
    }
    return false;
}

template<typename T>
T HwEGLExtLoader::load_func(void *handler, const char *funcName) {
    void *func = dlsym(handler, funcName);
    if (!func) {
        Logcat::e("HWVC", "HwAbsLibLoader could not find %s", funcName);
    } else {
        Logcat::i("HWVC", "HwAbsLibLoader dlsym(%s) succeeded.", funcName);
    }
    return reinterpret_cast<T>(func);
}