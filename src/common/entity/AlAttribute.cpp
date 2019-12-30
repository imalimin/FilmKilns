/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAttribute.h"

AlAttribute::AlAttribute(std::string &k, std::string &v) {
    pair.first = k;
    pair.second = v;
}

AlAttribute::AlAttribute(const AlAttribute &o) : Object() {
    pair.first = o.pair.first;
    pair.second = o.pair.second;
}

AlAttribute::~AlAttribute() {

}

std::string AlAttribute::name() {
    return pair.first;
}

std::string AlAttribute::value() {
    return pair.second;
}
