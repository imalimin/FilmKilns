/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKLAYERRENDERQUARK_H
#define FK_GRAPHIC_FKLAYERRENDERQUARK_H

#include "FkQuark.h"
#include "FkGraphicLayer.h"
#include "FkGraphicFrameObject.h"
#include "FkGraphicProgram.h"
#include "FkGraphicContext.h"
#include "FkGraphicRender.h"

FK_CLASS FkLayerRenderQuark FK_EXTEND FkQuark {
public:
    FkLayerRenderQuark();

    FkLayerRenderQuark(const FkLayerRenderQuark &o) = delete;

    virtual ~FkLayerRenderQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p);

private:
    FkResult _drawLayer(std::shared_ptr<FkGraphicLayer> layer,
                        std::shared_ptr<FkGraphicRender> render);

    FkSize _getCanvasSize(std::shared_ptr<FkGraphicLayer> canvas);

    std::shared_ptr<FkGraphicTexture> _getCanvasTexture(std::shared_ptr<FkGraphicLayer> canvas);

    FkResult _drawColor(std::shared_ptr<FkGraphicFrameObject> fbo,
                        std::shared_ptr<FkGraphicTexture> tex,
                        FkColor &color);

private:
    uint32_t vbo = 0;
};

#endif //FK_GRAPHIC_FKLAYERRENDERQUARK_H