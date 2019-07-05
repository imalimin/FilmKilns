/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwSequenceModel.h"

HwSequenceModel *HwSequenceModel::build() {
    return new HwSequenceModel();
}

HwSequenceModel::HwSequenceModel() : HwAbsPipelineModel(), pCodecConfig(HwCodecConfig::build()) {

}

HwSequenceModel::~HwSequenceModel() {
    if (pCodecConfig) {
        delete pCodecConfig;
        pCodecConfig = nullptr;
    }
}

HwCodecConfig *HwSequenceModel::getCodecConfig() { return pCodecConfig; }