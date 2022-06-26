/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICMATPROGRAM_H
#define FK_GRAPHIC_FKGRAPHICMATPROGRAM_H

#include "FkGraphicProgram.h"
#include "FkGLDefinition.h"
#include <vector>

FK_SUPER_CLASS(FkGraphicMatProgram, FkGraphicProgram) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicMatProgram)

public:
    FkGraphicMatProgram(const FkProgramDescription &desc);

    FkGraphicMatProgram(const FkGraphicMatProgram &o) = delete;

    virtual ~FkGraphicMatProgram();

    virtual FkProgramDescription::kType type() override;

    virtual FkResult create() override;

    virtual std::string getVertex() override;

    virtual std::string getFragment() override;

    virtual void clear() override;

    virtual FkResult addValue(std::shared_ptr<FkComponent> value) override;

private:
    std::vector<int32_t> uTexLocArray;
    int32_t uMVPMatLoc = -1;
    int32_t aPosLoc = -1;
    int32_t aCoordinateLoc = -1;
    int32_t uColsLoc = -1;
    int32_t uRowsLoc = -1;
    int32_t uWidthLoc = -1;
    int32_t uHeightLoc = -1;
    int32_t uSliceWidthLoc = -1;
    int32_t uSliceHeightLoc = -1;

};

#endif //FK_GRAPHIC_FKGRAPHICMATPROGRAM_H
