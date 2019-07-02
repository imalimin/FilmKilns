/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwUILoader.h"
#include "Logcat.h"

HwUILoader *HwUILoader::getInstance() {
    static HwUILoader instance;
    return &instance;
}

HwUILoader::HwUILoader() {
    if (open()) {
        load();
    }
    close();
}

HwUILoader::~HwUILoader() {

}

bool HwUILoader::open() {
    if (pLibHandler) {
        return true;
    }
    pLibHandler = dlopen(LIB_NAME.c_str(), RTLD_NOW);
    if (!pLibHandler) {
        Logcat::e("HWVC", "HwUILoader::open %s failed.", LIB_NAME.c_str());
        return false;
    }
    return true;
}

void HwUILoader::load() {
    void *ptr = load_func<void *>(pLibHandler,
                                  "_ZN7android13GraphicBufferC1Ejjij");
}

bool HwUILoader::close() {
    if (pLibHandler) {
        dlclose(pLibHandler);
        pLibHandler = nullptr;
        return true;
    }
    return false;
}

template<typename T>
T HwUILoader::load_func(void *handler, const char *funcName) {
    void *func = dlsym(handler, funcName);
    if (!func) {
        Logcat::e("HWVC", "HwUILoader could not find %s", funcName);
    } else {
        Logcat::i("HWVC", "HwUILoader dlsym(%s) succeeded.", funcName);
    }
    return reinterpret_cast<T>(func);
}
