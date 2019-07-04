/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwConfig.h"

HwConfig *HwConfig::build() {
    return new HwConfig();
}

HwConfig::HwConfig() : Object() {

}

HwConfig::~HwConfig() {

}