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
#include "FkGraphicTexComponent.h"
#include "FkCoordinateComponent.h"
#include "FkPositionComponent.h"
#include "FkMatrixComponent.h"
#include "FkMatrix.h"
#include "FkGLDefinition.h"
#include "FkGraphicFrameObject.h"
#include "FkColorComponent.h"
#include "FkSizeComponent.h"
#include "FkShapeComponent.h"
#include "FkVertexObjectComponent.h"

FK_CLASS FkGraphicRender FK_EXTEND FkObject, public enable_shared_from_this<FkGraphicRender> {
public:
    FK_ENUM kRenderMode {
        TRIANGLE_STRIP = 0,
        POINTS
    };
public:
    static std::shared_ptr<FkGraphicRender> with(std::shared_ptr<FkGraphicProgram> program);

    FkGraphicRender();

    FkGraphicRender(const FkGraphicRender &o);

    virtual ~FkGraphicRender();

    FkResult render(kRenderMode mode = kRenderMode::TRIANGLE_STRIP);

    std::shared_ptr<FkGraphicRender> enableSwapBuffers(bool enable);

    std::shared_ptr<FkGraphicRender> setContext(std::shared_ptr<FkGraphicContext> _context);

    std::shared_ptr<FkGraphicRender> setViewport(int x, int y, int width, int height);

    std::shared_ptr<FkGraphicRender> setColor(FkColor _color);

    std::shared_ptr<FkGraphicRender> setSrcTexture(int index, std::shared_ptr<FkGraphicTexture> tex);

    std::shared_ptr<FkGraphicRender> setVertexObj(std::shared_ptr<FkVertexObjectComponent> _vertexObj);

    std::shared_ptr<FkGraphicRender> setMatrix(std::shared_ptr<FkMatrixComponent> _comp);

    std::shared_ptr<FkGraphicRender> setFrameObject(std::shared_ptr<FkGraphicFrameObject> _fbo);

    std::shared_ptr<FkGraphicRender> setTargetTexture(std::shared_ptr<FkGraphicTexture> _tex);

    std::shared_ptr<FkGraphicRender> enableBlend(bool enable);

    std::shared_ptr<FkGraphicRender> setPointConfig(FkColor _color, FkShape::kType shape, int sizeOfPixel);

private:
    int _getRenderMode(kRenderMode mode);

private:
    std::shared_ptr<FkGraphicContext> context = nullptr;
    std::shared_ptr<FkColor> color = nullptr;
    FkSize size;
    std::shared_ptr<FkGraphicProgram> program = nullptr;
    std::shared_ptr<FkGraphicTexComponent> srcTex = nullptr;
    int32_t countVertex = 0;
    std::shared_ptr<FkVertexObjectComponent> vertexObj = nullptr;
    bool _enableSwapBuffers = false;
    bool _enableBlend = false;
    std::shared_ptr<FkMatrixComponent> mat;
    std::shared_ptr<FkGraphicFrameObject> fbo = nullptr;
    std::shared_ptr<FkGraphicTexture> targetTex = nullptr;
    std::shared_ptr<FkColorComponent> pointColor = nullptr;
    std::shared_ptr<FkSizeComponent> pointSizeOfPixel = nullptr;
    std::shared_ptr<FkShapeComponent> pointShape = nullptr;
};

#endif //FK_GRAPHIC_FKGRAPHICRENDER_H