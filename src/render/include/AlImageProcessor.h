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
#include "AlSize.h"
#include "AlRational.h"
#include <vector>

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

    HwResult removeLayer(int32_t id);

    HwResult moveLayerIndex(int32_t id, int32_t index);

    HwResult setScale(int32_t id, AlRational scale);

    HwResult postScale(int32_t id, AlRational ds);

    HwResult setRotation(int32_t id, AlRational r);

    HwResult postRotation(int32_t id, AlRational dr);

    HwResult setTranslate(int32_t id, float x, float y);

    HwResult postTranslate(int32_t id, float dx, float dy);

    HwResult setAlpha(int32_t id, float alpha);

    void calculatePosition(float &x, float &y);

    int32_t getLayer(float x, float y);

    HwResult save(std::string path);

private:
    void _notifyCanvasUpdate();

    void _notifyLayerUpdate();

    AlImageLayerModel *_getLayer(int32_t id);

private:
    const string ALIAS_OF_IMAGE = "IMAGE";
    const string ALIAS_OF_RENDER = "RENDER";
    const string ALIAS_OF_SCREEN = "SCREEN";
    AlImageCanvasModel mCanvasModel;
    std::vector<AlImageLayerModel *> mLayers;
    AlIdentityCreator mLayerIdCreator;
    std::mutex mLayerMtx;
    AlSize mWinSize;
};


#endif //HWVC_ANDROID_ALIMAGEPROCESSOR_H
