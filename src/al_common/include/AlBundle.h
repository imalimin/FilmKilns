/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALBUNDLE_H
#define HWVC_ANDROID_ALBUNDLE_H

#include "AlPrimitiveType.h"
#include <map>

AL_CLASS AlBundle AL_EXTEND Object {
public:
    AlBundle();

    AlBundle(const AlBundle &o);

    virtual ~AlBundle();

    bool put(std::string key, int32_t val);

    bool put(std::string key, int64_t val);

    bool put(std::string key, float val);

    bool put(std::string key, double val);

    bool put(std::string key, uint8_t val);

    bool put(std::string key, char val);

    bool put(std::string key, std::string val);

    int32_t get(std::string key, int32_t def);

    int64_t get(std::string key, int64_t def);

    float get(std::string key, float def);

    double get(std::string key, double def);

    uint8_t get(std::string key, uint8_t def);

    char get(std::string key, char def);

    std::string get(std::string key, std::string def);

    void remove(std::string key);

    bool contains(std::string key);

    virtual std::string toString() override;

private:
    std::map<std::string, std::unique_ptr<Object>> map;
    std::mutex mtx;
};


#endif //HWVC_ANDROID_ALBUNDLE_H
