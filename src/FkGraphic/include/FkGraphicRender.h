/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICRENDER_H
#define FK_GRAPHIC_FKGRAPHICRENDER_H

#include "FkObject.h"
#include "FkGraphicProgram.h"
#include "FkGraphicTexture.h"
#include "FkGraphicContext.h"
#include "FkTexValue.h"
#include "FkCoordinateValue.h"
#include "FkPositionValue.h"
#include "FkMatrixValue.h"
#include "FkMatrix.h"
#include "FkGLDefinition.h"

FK_CLASS FkGraphicRender FK_EXTEND FkObject, public enable_shared_from_this<FkGraphicRender> {
public:
    static std::shared_ptr<FkGraphicRender> with(std::shared_ptr<FkGraphicProgram> program);

    FkGraphicRender();

    FkGraphicRender(const FkGraphicRender &o);

    virtual ~FkGraphicRender();

    FkResult render();

    std::shared_ptr<FkGraphicRender> enableSwapBuffers(bool enable);

    std::shared_ptr<FkGraphicRender> setContext(std::shared_ptr<FkGraphicContext> _context);

    std::shared_ptr<FkGraphicRender> setViewport(int x, int y, int width, int height);

    std::shared_ptr<FkGraphicRender> setColor(FkColor _color);

    std::shared_ptr<FkGraphicRender> setVertexBuffer(uint32_t _vbo);

    std::shared_ptr<FkGraphicRender> setSrcTexture(int index, std::shared_ptr<FkGraphicTexture> tex);

    std::shared_ptr<FkGraphicRender> setPosition(size_t countVertex, size_t countPerVertex, size_t offset, void *data);

    std::shared_ptr<FkGraphicRender> setCoordinate(size_t countVertex, size_t countPerVertex, size_t offset, void *data);

    std::shared_ptr<FkGraphicRender> setMatrix(std::shared_ptr<FkMatrix> _mat);

private:
    std::shared_ptr<FkGraphicContext> context = nullptr;
    FkColor color;
    FkSize size;
    std::shared_ptr<FkGraphicProgram> program = nullptr;
    uint32_t vbo = GL_NONE;
    std::shared_ptr<FkTexValue> srcTex = nullptr;
    std::shared_ptr<FkPositionValue> position = nullptr;
    std::shared_ptr<FkCoordinateValue> coordinate = nullptr;
    bool _enableSwapBuffers = false;
    std::shared_ptr<FkMatrixValue> mat;
};

#endif //FK_GRAPHIC_FKGRAPHICRENDER_H