/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-4-26 13:18:24
*/

#ifndef FK_GRAPHIC_FKCANVASMATPROGRAM_H
#define FK_GRAPHIC_FKCANVASMATPROGRAM_H

#include "FkGraphicMatProgram.h"

FK_SUPER_CLASS(FkCanvasMatProgram, FkGraphicMatProgram) {
FK_DEF_CLASS_TYPE_FUNC(FkCanvasMatProgram)

public:
    FkCanvasMatProgram(const FkProgramDescription &desc);

    FkCanvasMatProgram(const FkCanvasMatProgram &o) = delete;

    virtual ~FkCanvasMatProgram();

    virtual FkProgramDescription::kType type() override;

    virtual FkResult create() override;

    virtual FkResult addValue(std::shared_ptr<FkComponent> value) override;

    virtual std::string getFragment() override;

private:
    int32_t uGridSizeXLoc = -1;
    int32_t uGridSizeYLoc = -1;
};

#endif //FK_GRAPHIC_FKCANVASMATPROGRAM_H