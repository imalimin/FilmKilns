/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALFUTURE_H
#define HWVC_ANDROID_ALFUTURE_H

#include "AlBundle.h"
#include "SimpleLock.h"

AL_CLASS AlFuture AL_EXTEND Object {
public:
    std::shared_ptr<Object> sp;

public:
    AlFuture();

    virtual ~AlFuture();

    bool put(int32_t val);

    bool put(int64_t val);

    bool put(float val);

    bool put(double val);

    bool put(uint8_t val);

    bool put(char val);

    bool put(std::string val);

    int32_t get(int32_t def);

    int64_t get(int64_t def);

    float get(float def);

    double get(double def);

    uint8_t get(uint8_t def);

    char get(char def);

    std::string get(std::string def);

private:
    AlFuture(const AlFuture &o) : Object() {
    };

private:
    std::mutex mtx;
    std::condition_variable cond;
    AlBundle bundle;
    bool done = false;
};


#endif //HWVC_ANDROID_ALFUTURE_H
