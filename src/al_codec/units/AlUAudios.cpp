/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUAudios.h"

AlUAudios::AlUAudios(const std::string alias) : Unit(alias) {

}

AlUAudios::~AlUAudios() {

}

bool AlUAudios::onCreate(AlMessage *msg) {
    return true;
}

bool AlUAudios::onDestroy(AlMessage *msg) {
    return true;
}