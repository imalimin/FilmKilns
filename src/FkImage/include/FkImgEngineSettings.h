/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-5-1 10:59:07
*/

#ifndef FK_IMAGE_FKIMGENGINESETTINGS_H
#define FK_IMAGE_FKIMGENGINESETTINGS_H

#include "FkEngineSettings.h"

FK_SUPER_CLASS(FkImgEngineSettings, FkEngineSettings) {
FK_DEF_CLASS_TYPE_FUNC(FkImgEngineSettings)

public:
    FkImgEngineSettings();

    FkImgEngineSettings(const FkImgEngineSettings &o);

    virtual ~FkImgEngineSettings();
};

#endif //FK_IMAGE_FKIMGENGINESETTINGS_H