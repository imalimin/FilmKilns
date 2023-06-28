/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-08 16:14:18
*/

#ifndef FK_GRAPHIC_FKGRAPHICPATHPROGRAM_H
#define FK_GRAPHIC_FKGRAPHICPATHPROGRAM_H

#include "FkGraphicProgram.h"
#include "FkSize.h"

FK_SUPER_CLASS(FkGraphicPathProgram, FkGraphicProgram) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicPathProgram)

public:
    FkGraphicPathProgram(const FkProgramDescription &desc);

    FkGraphicPathProgram(const FkGraphicPathProgram &o) = delete;

    virtual ~FkGraphicPathProgram();

    virtual FkProgramDescription::kType type() override;

    virtual FkResult create() override;

    virtual void clear() override;

    virtual FkResult addValue(std::shared_ptr<FkComponent> value) override;

public:
    virtual std::string getVertex() override;

    virtual std::string getFragment() override;

private:
    int32_t aPosLoc = -1;
    int32_t uPaintColorLoc = -1;
    int32_t uViewportMatLoc = -1;
    FkSize size;
    std::vector<float> vertexes;
};

#endif //FK_GRAPHIC_FKGRAPHICPATHPROGRAM_H