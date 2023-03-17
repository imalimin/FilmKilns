/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-17 22:30:34
*/

#ifndef FK_GRAPHIC_FKRENDERENGINESETTINGS_H
#define FK_GRAPHIC_FKRENDERENGINESETTINGS_H

#include "FkEngineSettings.h"
#include <vector>

FK_SUPER_CLASS(FkRenderEngineSettings, FkEngineSettings) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderEngineSettings)

public:
    FkRenderEngineSettings();

    FkRenderEngineSettings(const FkRenderEngineSettings &o);

    virtual ~FkRenderEngineSettings();

    virtual void setSupportBlockSizeArray(std::vector<int> &vec);

    virtual std::vector<int> getSupportBlockSizeArray();

private:
    std::vector<int> supportBlockArray;
};

#endif //FK_GRAPHIC_FKRENDERENGINESETTINGS_H