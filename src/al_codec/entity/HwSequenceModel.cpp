/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwSequenceModel.h"

HwSequenceModel *HwSequenceModel::build() {
    return new HwSequenceModel();
}

HwSequenceModel::HwSequenceModel() : Object(), pCodecConfig(HwCodecConfig::build()) {

}

HwSequenceModel::~HwSequenceModel() {
    if (pCodecConfig) {
        delete pCodecConfig;
        pCodecConfig = nullptr;
    }
}

HwCodecConfig *HwSequenceModel::getCodecConfig() { return pCodecConfig; }