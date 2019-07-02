/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWUILOADER_H
#define HWVC_ANDROID_HWUILOADER_H

#include "../HwAbsLibLoader.h"

class HwUILoader : public HwAbsLibLoader {
public:
    static HwUILoader *getInstance();

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
