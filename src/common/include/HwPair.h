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
    typedef pair<K, V> Pair;
    Pair p;

public:
    HwPair(K key, V value) {
        p = pair<K, V>(key, value);
    }

    HwPair(const HwPair &hwPair) {
        this->p = pair<K, V>(hwPair.key(), hwPair.value());
    }

    virtual ~HwPair() {

    }

    K key() const {
        return p.first;
    }

    V value() const {
        return p.second;
    }
};


#endif //HWVC_ANDROID_PAIR_H
