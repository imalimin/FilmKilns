/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 * Project hwvc(https://github.com/lmylr/hwvc).
 */

#ifndef HWVC_ANDROID_HWPOINTER_H
#define HWVC_ANDROID_HWPOINTER_H


template<class T>
class HwPointer {
public:
    HwPointer(T *ptr) {
        this->ptr = ptr;
        if (ptr) {
            ptr->ref();
        }
    }

    ~HwPointer() {
        if (ptr) {
            ptr->unref();
        }
    }

private:
    T *ptr;
};


#endif //HWVC_ANDROID_HWPOINTER_H
