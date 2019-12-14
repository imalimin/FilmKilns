/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALLAYERDESCRIPTOR_H
#define HWVC_ANDROID_ALLAYERDESCRIPTOR_H

#include "Unit.h"
#include "AlImageLayer.h"
#include "AlLayerMeasure.h"
#include "HwResult.h"
#include "AlSize.h"

class AlLayerDescriptor : public Unit {
public:
    AlLayerDescriptor(const string &alias);

    virtual ~AlLayerDescriptor();

    bool onCreate(Message *msg) override;

    bool onDestroy(Message *msg) override;

    bool onMeasure(Message *msg);

    bool onCanvasSizeUpdate(Message *msg);

private:
    /// Measure all params for canvas
    /// \param drawModel
    /// \return {@link HwResult}
    HwResult _measure(AlImageLayer *layer, AlImageLayerDrawModel *description);

    /// Measure all operate`s params for canvas
    /// \param drawModel
    /// \return {@link HwResult}
    HwResult _measureOperate(AlImageLayer *layer, AlImageLayerDrawModel *description);

    void notifyCanvas(AlImageLayerDrawModel *description);

private:
    AlLayerMeasure aMeasure;
    AlSize aCanvasSize;
};


#endif //HWVC_ANDROID_ALLAYERDESCRIPTOR_H
