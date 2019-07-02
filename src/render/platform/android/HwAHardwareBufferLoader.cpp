/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwAHardwareBufferLoader.h"
#include <dlfcn.h>
#include "Logcat.h"

HwAHardwareBufferLoader *HwAHardwareBufferLoader::getInstance() {
    static HwAHardwareBufferLoader instance;
    return &instance;
}

HwAHardwareBufferLoader::HwAHardwareBufferLoader() : HwAbsLibLoader() {
    if (open()) {
        load();
    }
    close();
}

HwAHardwareBufferLoader::~HwAHardwareBufferLoader() {
}

bool HwAHardwareBufferLoader::open() {
    if (pLibHandler) {
        return true;
    }
    pLibHandler = dlopen(LIB_NAME.c_str(), 0);
    if (!pLibHandler) {
        Logcat::e("HWVC", "HwAHardwareBufferLoader::open %s failed.", LIB_NAME.c_str());
        return false;
    }
    return true;
}

void HwAHardwareBufferLoader::load() {
    FAHardwareBuffer_allocate = load_func<signature_I_PDPB>(pLibHandler,
                                                            "AHardwareBuffer_allocate");
    FAHardwareBuffer_describe = load_func<signature_V_PBPD>(pLibHandler,
                                                            "AHardwareBuffer_describe");
    FAHardwareBuffer_lock = load_func<signature_I_PBJIPRP>(pLibHandler, "AHardwareBuffer_lock");
    FAHardwareBuffer_unlock = load_func<signature_I_PBPI>(pLibHandler, "AHardwareBuffer_unlock");
    FAHardwareBuffer_release = load_func<signature_V_PB>(pLibHandler, "AHardwareBuffer_release");
}

bool HwAHardwareBufferLoader::close() {
    if (pLibHandler) {
        dlclose(pLibHandler);
        pLibHandler = nullptr;
        return true;
    }
    return false;
}

template<typename T>
T HwAHardwareBufferLoader::load_func(void *handler, const char *funcName) {
    void *func = dlsym(handler, funcName);
    if (!func) {
        Logcat::e("HWVC", "HwAbsLibLoader could not find %s", funcName);
    } else {
        Logcat::i("HWVC", "HwAbsLibLoader dlsym(%s) succeeded.", funcName);
    }
    return reinterpret_cast<T>(func);
}