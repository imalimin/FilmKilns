/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWAHARDWAREBUFFERLOADER_H
#define HWVC_ANDROID_HWAHARDWAREBUFFERLOADER_H

#include "../HwAbsLibLoader.h"
#include <android/hardware_buffer.h>

using namespace std;

class HwAHardwareBufferLoader : public HwAbsLibLoader {
public:
    static HwAHardwareBufferLoader *getInstance();

    typedef int (*signature_I_PDPB)(const AHardwareBuffer_Desc *, AHardwareBuffer **);

    typedef void (*signature_V_PBPD)(const AHardwareBuffer *, AHardwareBuffer_Desc *);

    typedef void (*signature_I_PBJIPRP)(AHardwareBuffer *, uint64_t, int32_t, const ARect *,
                                        void **);

    typedef void (*signature_I_PBPI)(AHardwareBuffer *, int32_t *);

    typedef void (*signature_V_PB)(AHardwareBuffer *);

    signature_I_PDPB FAHardwareBuffer_allocate;
    signature_V_PBPD FAHardwareBuffer_describe;
    signature_I_PBJIPRP FAHardwareBuffer_lock;
    signature_I_PBPI FAHardwareBuffer_unlock;
    signature_V_PB FAHardwareBuffer_release;

protected:
    bool open() override;

    void load() override;

    bool close() override;

    template<typename T>
    static T load_func(void *handler, const char *funcName);
private:
    HwAHardwareBufferLoader();

    ~HwAHardwareBufferLoader();

private:
    const string LIB_NAME = "libandroid.so";
    void *pLibHandler = nullptr;
};


#endif //HWVC_ANDROID_HWAHARDWAREBUFFERLOADER_H
