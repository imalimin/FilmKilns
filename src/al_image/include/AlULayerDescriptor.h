/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALULAYERDESCRIPTOR_H
#define HWVC_ANDROID_ALULAYERDESCRIPTOR_H

#include "Unit.h"
#include "AlImageLayer.h"
#include "AlLayerMeasurer.h"
#include "HwResult.h"
#include "AlSize.h"
#include "AlImgLayerDescription.h"

al_def_unit(AlULayerDescriptor, Unit) {
public:
    AlULayerDescriptor(const string &alias);

    virtual ~AlULayerDescriptor();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    /// 对Layer进行测量
    /// FORMAT:
    /// +------------------------------------------------------------------------+
    /// | msg::obj                | msg::arg1            | msg::arg2 | msg::desc |
    /// +------------------------------------------------------------------------+
    /// | AlImageLayer ObjectBox | Just render to screen | none      | none     |
    /// +------------------------------------------------------------------------+
    /// \param msg
    /// \return
    bool onMeasure(AlMessage *msg);

    bool onCanvasSizeUpdate(AlMessage *msg);

private:
    /// Measure all params for canvas
    /// \param drawModel
    /// \return {@link HwResult}
    HwResult _measure(AlImageLayer *layer,
                      AlImageLayerModel *layerModel,
                      AlImageLayerDrawModel *description);

    /// Measure all operate`s params for canvas
    /// \param model
    /// \param description
    /// \return {@link HwResult}
    HwResult _measureOperate(std::vector<AlAbsMAction *> *opts,
                             AlImgLayerDescription &model,
                             AlImageLayerDrawModel *description);

    void notifyCanvas(AlImageLayerDrawModel *description, int32_t flags);

    void _measureLayerQuad(AlImageLayerModel *model, AlSize &size);

    void _updateCanvasSize(int32_t width, int32_t height);

private:
    AlSize aCanvasSize;
    AlOrthMatrix oMat;
    AlMatrix tMat;
};


#endif //HWVC_ANDROID_ALULAYERDESCRIPTOR_H
