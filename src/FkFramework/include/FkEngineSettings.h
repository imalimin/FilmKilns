/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-5-1 10:58:01
*/

#ifndef FK_FRAMEWORK_FKENGINESETTINGS_H
#define FK_FRAMEWORK_FKENGINESETTINGS_H

#include "FkObject.h"

FK_SUPER_CLASS(FkEngineSettings, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkEngineSettings)

public:
    FkEngineSettings();

    FkEngineSettings(const FkEngineSettings &o);

    virtual ~FkEngineSettings();

public:
    bool enableEngineThread = true;
};

#endif //FK_FRAMEWORK_FKENGINESETTINGS_H