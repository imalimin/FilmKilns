/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICMOSAICPROGRAM_H
#define FK_GRAPHIC_FKGRAPHICMOSAICPROGRAM_H

#include "FkGraphicProgram.h"
#include "FkGLDefinition.h"

FK_SUPER_CLASS(FkCanvasMosaicProgram, FkGraphicProgram) {
FK_DEF_CLASS_TYPE_FUNC(FkCanvasMosaicProgram)

public:
    FkCanvasMosaicProgram(const FkProgramDescription &desc);

    FkCanvasMosaicProgram(const FkCanvasMosaicProgram &o) = delete;

    virtual ~FkCanvasMosaicProgram();

    virtual FkProgramDescription::kType type() override;

    virtual FkResult create() override;

    virtual std::string getVertex() override;

    virtual std::string getFragment() override;

    virtual void clear() override;

    virtual FkResult addValue(std::shared_ptr<FkComponent> value) override;

private:
    int32_t aPosLoc = -1;
    int32_t aCoordinateLoc = -1;
};

#endif //FK_GRAPHIC_FKGRAPHICMOSAICPROGRAM_H