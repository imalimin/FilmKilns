/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-17 22:30:34
*/

#include "FkRenderEngineSettings.h"

FK_IMPL_CLASS_TYPE(FkRenderEngineSettings, FkEngineSettings)

FkRenderEngineSettings::FkRenderEngineSettings() : FkEngineSettings() {
    supportBlockArray = {4096, 1024, 512};
}

FkRenderEngineSettings::FkRenderEngineSettings(const FkRenderEngineSettings &o)
        : FkEngineSettings(o),
          supportBlockArray(o.supportBlockArray) {

}

FkRenderEngineSettings::~FkRenderEngineSettings() {

}

void FkRenderEngineSettings::setSupportBlockSizeArray(std::vector<int> &vec) {
    supportBlockArray = vec;
}

std::vector<int> FkRenderEngineSettings::getSupportBlockSizeArray() {
    return supportBlockArray;
}