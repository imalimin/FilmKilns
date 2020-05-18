/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALSINGLETON_H
#define HWVC_ANDROID_ALSINGLETON_H

#import "Object.h"

template<typename T>
al_class AlSingleton al_extend Object {
public:
    static T *instance() {
        return _instance;
    }

private:
    AlSingleton() : Object() {}

    AlSingleton(const AlSingleton &o) : Object() {}

    ~AlSingleton() {}

private:
    static T *_instance = new T();
};

#endif //HWVC_ANDROID_ALSINGLETON_H
