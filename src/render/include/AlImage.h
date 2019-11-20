/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGE_H
#define HWVC_ANDROID_ALIMAGE_H

#include "Unit.h"
#include "TextureAllocator.h"
#include "HwAbsTexture.h"
#include "AlImageCanvas.h"
#include "AlImageCanvasModel.h"
#include "AlImageLayerManager.h"
#include <list>

class AlImage : public Unit {
public:
    AlImage(string alias);

    virtual ~AlImage();

    bool eventRelease(Message *msg) override;

    bool onPrepare(Message *msg);

    bool onNewLayer(Message *msg);

    bool onInvalidate(Message *m);

    bool onUpdateCanvas(Message *m);

private:
    void _newDefaultCanvas();

private:
    TextureAllocator *texAllocator = nullptr;

    /*+------------------+*/
    /*|     Model        |*/
    /*+------------------+*/
private:
    AlImageCanvasModel *getCanvas();

    std::list<AlImageLayerModel *> *getLayers();

    AlImageCanvas mCanvas;
    AlImageLayerManager mLayerManager;
};


#endif //HWVC_ANDROID_ALIMAGE_H
