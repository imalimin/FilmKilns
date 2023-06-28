/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-31 23:36:53
*/

#ifndef FKGRAPHIC_FKNORMALPROGRAM_H
#define FKGRAPHIC_FKNORMALPROGRAM_H

#include "FkGraphicProgram.h"

FK_SUPER_CLASS(FkNormalProgram, FkGraphicProgram) {
FK_DEF_CLASS_TYPE_FUNC(FkNormalProgram)

public:
    FkNormalProgram(const FkProgramDescription &desc);

    FkNormalProgram(const FkNormalProgram &o) = delete;

    virtual ~FkNormalProgram();


    virtual FkProgramDescription::kType type() override;

    virtual FkResult create() override;

    virtual std::string getVertex() override;

    virtual std::string getFragment() override;

    virtual void clear() override;

    virtual FkResult addValue(std::shared_ptr<FkComponent> value) override;

private:
    int32_t uTexLoc;
    int32_t uMVPMatLoc = -1;
    int32_t uViewportMatLoc = -1;
    int32_t aPosLoc = -1;
    int32_t aCoordinateLoc = -1;
};

#endif //FKGRAPHIC_FKNORMALPROGRAM_H