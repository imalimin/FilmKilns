/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
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
    fGraphicBufferCtor = load_func<pfnGraphicBufferCtor>(pLibHandler,
                                                         "_ZN7android13GraphicBufferC1Ejjij");
    fGraphicBufferAndroid7Ctor = load_func<pfGraphicBufferAndroid7Ctor>(pLibHandler,
                                                                        "_ZN7android13GraphicBufferC1EjjijNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEE");
    fGraphicBufferDtor = load_func<pfnGraphicBufferDtor>(pLibHandler,
                                                         "_ZN7android13GraphicBufferD1Ev");
    fGraphicBufferLock = (pfnGraphicBufferLock) dlsym(pLibHandler,
                                                      "_ZN7android13GraphicBuffer4lockEjPPv");
    fGraphicBufferLockRect = (pfnGraphicBufferLockRect) dlsym(pLibHandler,
                                                              "_ZN7android13GraphicBuffer4lockEjRKNS_4RectEPPv");
    fGraphicBufferUnlock = (pfnGraphicBufferUnlock) dlsym(pLibHandler,
                                                          "_ZN7android13GraphicBuffer6unlockEv");
    fGraphicBufferGetNativeBuffer = (pfnGraphicBufferGetNativeBuffer) dlsym(pLibHandler,
                                                                            "_ZNK7android13GraphicBuffer15getNativeBufferEv");
    fGraphicBufferReallocate = (pfnGraphicBufferReallocate) dlsym(pLibHandler,
                                                                  "_ZN7android13GraphicBuffer10reallocateEjjij");
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
