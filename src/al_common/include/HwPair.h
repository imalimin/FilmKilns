/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_PAIR_H
#define HWVC_ANDROID_PAIR_H

#include "Object.h"
#include <string>
#include <map>

using namespace std;

template<typename K, typename V>
class HwPair : public Object {
private:
    K first;
    V second;

public:
    HwPair(K key, V value) : Object(), first(key), second(value) {

    }

    HwPair(const HwPair &hwPair) : Object(), first(hwPair.first), second(hwPair.second) {
    }

    virtual ~HwPair() {

    }

    K key() const {
        return first;
    }

    V value() const {
        return second;
    }
};


#endif //HWVC_ANDROID_PAIR_H
