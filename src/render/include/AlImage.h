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

class AlImage : public Unit {
public:
    ///code, msg, path
    typedef function<void(int32_t, const char *, const char *)> OnSaveListener;
public:
    AlImage(string alias);

    virtual ~AlImage();

    bool eventRelease(Message *msg) override;

    bool onPrepare(Message *msg);

    bool onUpdateLayer(Message *msg);

    bool onInvalidate(Message *m);

    bool onUpdateCanvas(Message *m);

    bool onSave(Message *m);

    void setOnSaveListener(OnSaveListener listener);

private:
    void _newDefaultCanvas();

    void _drawAllLayer(bool transparent = false);

private:
    TextureAllocator *texAllocator = nullptr;

    /*+------------------+*/
    /*|     Model        |*/
    /*+------------------+*/
private:
    AlImageCanvasModel *getCanvas();

    std::vector<AlImageLayerModel *> *getLayers();

    AlImageCanvas mCanvas;
    AlImageLayerManager mLayerManager;
    OnSaveListener onSaveListener = nullptr;
};


#endif //HWVC_ANDROID_ALIMAGE_H
