/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-16 22:11:03
*/

#ifndef FK_GRAPHIC_FKGRAPHICMATANDROIDPROGRAM_H
#define FK_GRAPHIC_FKGRAPHICMATANDROIDPROGRAM_H

#include "FkGraphicProgram.h"

FK_SUPER_CLASS(FkGraphicMatAndroidProgram, FkGraphicProgram) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicMatAndroidProgram)

public:
    FkGraphicMatAndroidProgram(const FkProgramDescription &desc);

    FkGraphicMatAndroidProgram(const FkGraphicMatAndroidProgram &o) = delete;

    virtual ~FkGraphicMatAndroidProgram();

    virtual FkProgramDescription::kType type() override;

    virtual FkResult create() override;

    virtual std::string getVertex() override;

    virtual std::string getFragment() override;

    virtual void clear() override;

    virtual FkResult addValue(std::shared_ptr<FkComponent> value) override;

private:
    int32_t uTexLoc;
    int32_t uMVPMatLoc = -1;
    int32_t uTransMatrixLoc = -1;
    int32_t aPosLoc = -1;
    int32_t aCoordinateLoc = -1;
};

#endif //FK_GRAPHIC_FKGRAPHICMATANDROIDPROGRAM_H