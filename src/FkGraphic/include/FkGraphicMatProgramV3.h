/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-19 16:17:35
*/

#ifndef FK_GRAPHIC_FKGRAPHICMATPROGRAMV3_H
#define FK_GRAPHIC_FKGRAPHICMATPROGRAMV3_H

#include "FkGraphicMatProgram.h"

FK_SUPER_CLASS(FkGraphicMatProgramV3, FkGraphicMatProgram) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicMatProgramV3)

public:
    FkGraphicMatProgramV3(const FkProgramDescription &desc);

    FkGraphicMatProgramV3(const FkGraphicMatProgramV3 &o) = delete;

    virtual ~FkGraphicMatProgramV3();

    virtual FkProgramDescription::kType type() override;

    virtual FkResult create() override;

    virtual std::string getVertex() override;

    virtual std::string getFragment() override;

    virtual void clear() override;

    virtual FkResult addValue(std::shared_ptr<FkComponent> value) override;

private:
    int32_t aPosLoc = -1;
    int32_t aCoordinateLoc = -1;
    int32_t uMVPMatLoc = -1;
    std::vector<int32_t> uTexLocArray;
    int32_t uColsLoc = -1;
    int32_t uRowsLoc = -1;
    int32_t uOffsetWidthLoc = -1;
    int32_t uOffsetHeightLoc = -1;
};

#endif //FK_GRAPHIC_FKGRAPHICMATPROGRAMV3_H