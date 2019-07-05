/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwCodecConfig.h"

HwCodecConfig *HwCodecConfig::build() {
    return new HwCodecConfig();
}

HwCodecConfig::HwCodecConfig() : HwConfig() {

}

HwCodecConfig::~HwCodecConfig() {

}