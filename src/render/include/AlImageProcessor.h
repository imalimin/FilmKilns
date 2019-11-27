/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGEPROCESSOR_H
#define HWVC_ANDROID_ALIMAGEPROCESSOR_H

#include "HwAbsProcessor.h"
#include "HwResult.h"
#include "HwWindow.h"
#include "AlImageCanvasModel.h"
#include "AlImageLayerModel.h"
#include <list>

class AlImageProcessor : public HwAbsProcessor {
public:
    AlImageProcessor();

    ~AlImageProcessor();

    virtual void onDestroy() override;

    void prepare(HwWindow *win);

    void updateWindow(HwWindow *win);

    void setCanvas(int32_t w, int32_t h, int32_t color);

    void invalidate();

    int32_t addLayer(const char *path);

    int32_t removeLayer(int32_t id);

    int32_t setScale(int32_t id, float scale);

    int32_t setRotation(int32_t id, float rotation);

    int32_t setTranslate(int32_t id, float x, float y);

    int32_t setAlpha(int32_t id, float alpha);

private:
    void _notifyCanvasUpdate();

    void _notifyLayerUpdate();

    AlImageLayerModel *_getLayer(int32_t id);

private:
    const string ALIAS_OF_IMAGE = "IMAGE";
    const string ALIAS_OF_RENDER = "RENDER";
    const string ALIAS_OF_SCREEN = "SCREEN";
    AlImageCanvasModel mCanvasModel;
    std::list<AlImageLayerModel *> mLayers;
    AlIdentityCreator mLayerIdCreator;
    std::mutex mLayerMtx;
};


#endif //HWVC_ANDROID_ALIMAGEPROCESSOR_H
