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

FK_CLASS FkGraphicMatProgram FK_EXTEND FkGraphicProgram {
public:
    FkGraphicMatProgram(const FkProgramDescription &desc);

    FkGraphicMatProgram(const FkGraphicMatProgram &o) = delete;

    virtual ~FkGraphicMatProgram();

    virtual FkResult create() override;

    virtual std::string getVertex() override;

    virtual std::string getFragment() override;

    virtual void clear() override;

    virtual FkResult addValue(std::shared_ptr<FkProgramValue> value) override;

private:
    int32_t uTextureLocation = GL_NONE;
    int32_t uModelMatLoc = -1;
    int32_t uViewMatLoc = -1;
    int32_t uProjMatLoc = -1;
    int32_t aPositionLocation = GL_NONE;
    int32_t aTextureCoordinateLocation = GL_NONE;

};

#endif //FK_GRAPHIC_FKGRAPHICMATPROGRAM_H
