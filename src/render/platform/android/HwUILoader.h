/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWUILOADER_H
#define HWVC_ANDROID_HWUILOADER_H

#include "../HwAbsLibLoader.h"
#include <android/hardware_buffer.h>

class HwUILoader : public HwAbsLibLoader {
public:
    static HwUILoader *getInstance();

    typedef void (*pfnGraphicBufferCtor)(void *, uint32_t w, uint32_t h, uint32_t format,
                                       uint32_t usage);

    pfnGraphicBufferCtor fGraphicBufferCtor;

    typedef void (*pfnGraphicBufferDtor)(void *);

    pfnGraphicBufferDtor fGraphicBufferDtor;

    typedef int (*pfnGraphicBufferLock)(void *, uint32_t usage, unsigned char **addr);

    pfnGraphicBufferLock fGraphicBufferLock;

    typedef int (*pfnGraphicBufferLockRect)(void *, uint32_t usage, const ARect &,
                                            unsigned char **addr);

    pfnGraphicBufferLockRect fGraphicBufferLockRect;

    typedef int (*pfnGraphicBufferUnlock)(void *);

    pfnGraphicBufferUnlock fGraphicBufferUnlock;

    typedef void *(*pfnGraphicBufferGetNativeBuffer)(void *);

    pfnGraphicBufferGetNativeBuffer fGraphicBufferGetNativeBuffer;

    typedef int (*pfnGraphicBufferReallocate)(void *, uint32_t w, uint32_t h, uint32_t format);

    pfnGraphicBufferReallocate fGraphicBufferReallocate;

protected:
    bool open() override;

    void load() override;

    bool close() override;

    template<typename T>
    static T load_func(void *handler, const char *funcName);

private:
    HwUILoader();

    ~HwUILoader();

private:
    const string LIB_NAME = "libui.so";
    void *pLibHandler = nullptr;

};


#endif //HWVC_ANDROID_HWUILOADER_H
