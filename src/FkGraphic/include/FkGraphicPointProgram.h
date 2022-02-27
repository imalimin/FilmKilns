/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICPOINTPROGRAM_H
#define FK_GRAPHIC_FKGRAPHICPOINTPROGRAM_H

#include "FkGraphicProgram.h"

FK_CLASS FkGraphicPointProgram FK_EXTEND FkGraphicProgram {
public:
    FkGraphicPointProgram(const FkProgramDescription &desc);

    FkGraphicPointProgram(const FkGraphicPointProgram &o) = delete;

    virtual ~FkGraphicPointProgram();

    virtual FkResult create() override;

    virtual void clear() override;

    virtual FkResult addValue(std::shared_ptr<FkComponent> value) override;

public:
    virtual std::string getVertex() override;

    virtual std::string getFragment() override;

private:
    int32_t aPosLoc = -1;
    int32_t uSizeLoc = -1;
    int32_t uColorLoc = -1;
    int32_t uModeLoc = -1;
};

#endif //FK_GRAPHIC_FKGRAPHICPOINTPROGRAM_H