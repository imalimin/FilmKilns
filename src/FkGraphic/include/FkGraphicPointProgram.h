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

    virtual FkResult addValue(std::shared_ptr<FkProgramValue> value) override;

public:
    virtual std::string getVertex() override;

    virtual std::string getFragment() override;
};

#endif //FK_GRAPHIC_FKGRAPHICPOINTPROGRAM_H