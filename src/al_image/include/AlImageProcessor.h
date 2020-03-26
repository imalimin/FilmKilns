/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGEPROCESSOR_H
#define HWVC_ANDROID_ALIMAGEPROCESSOR_H

#include "AlAbsProcessor.h"
#include "HwResult.h"
#include "HwWindow.h"
#include "AlImageCanvasModel.h"
#include "AlImageLayerModel.h"
#include "AlSize.h"
#include "AlRational.h"
#include "AlUCanvas.h"
#include "AlAbsContext.h"
#include "AlEgl.h"
#include "Al2DCoordinate.h"
#include <vector>

al_class_ex(AlImageProcessor, AlAbsProcessor) {
public:
    AlImageProcessor();

    ~AlImageProcessor();

    virtual void onCreate() override;

    virtual void onDestroy() override;

    void updateWindow(HwWindow *win);

    void setCanvas(int32_t w, int32_t h, int32_t color);

    /// 通知所有单元进行绘制操作
    /// \param msg
    /// \return
    void invalidate(int32_t flag = 0);

    int32_t addLayer(const char *path);

    HwResult removeLayer(int32_t id);

    HwResult moveLayerIndex(int32_t id, int32_t index);

    HwResult setScale(int32_t id, AlRational scale);

    HwResult postScale(int32_t id, AlRational ds, AlPointF anchor);

    HwResult setRotation(int32_t id, AlRational r);

    HwResult postRotation(int32_t id, AlRational dr, AlPointF anchor);

    HwResult setTranslate(int32_t id, float x, float y);

    HwResult postTranslate(int32_t id, float dx, float dy);

    HwResult setAlpha(int32_t id, float alpha);

    void transToCanvasPos(float &x, float &y);

    /// 根据window坐标获取对应的Layer，还需要适配缩放旋转的情况
    /// \param x x坐标，x∈[-1,1]
    /// \param y y坐标，y∈[-1,1]
    /// \return 对应的图层id，否则-1
    int32_t getLayer(float x, float y);

    HwResult cropLayer(int32_t id, float left, float top, float right, float bottom);

    /// 裁剪画布，并保持各图层在画布中的位置不变
    /// \param left
    /// \param top
    /// \param right
    /// \param bottom
    /// \return
    HwResult cropCanvas(float left, float top, float right, float bottom);

    HwResult cancelCropLayer(int32_t id);

    HwResult ensureAlignCrop(int32_t id, AlRational r);

    HwResult cancelAlignCrop(int32_t id);

    HwResult save(std::string path);

    /// 导出到.qua文件
    /// \param path
    /// \return
    HwResult exportFile(std::string path);

    /// 从.qua文件导入
    /// \param path
    /// \return
    HwResult importFile(std::string path);

    HwResult redo();

    HwResult undo();

    /// 画笔绘制接口
    /// \param id
    /// \param pointF
    /// \param painting
    /// \return
    HwResult paint(int32_t id, float x, float y, bool painting);

    void setOnSaveListener(AlUCanvas::OnSaveListener listener);

private:
    void _notifyCanvasUpdate();

    void _notifyLayerUpdate();

    AlImageLayerModel *_findLayer(int32_t id);

    bool _onCanvasUpdate(AlMessage *msg);

private:
    const string ALIAS_OF_LAYER = "LAYER";
    const string ALIAS_OF_FILTER = "LAYER_FILTER";
    const string ALIAS_OF_DESCRIPTOR = "LAYER_DESCRIPTOR";
    const string ALIAS_OF_CANVAS = "LAYER_CANVAS";
    const string ALIAS_OF_SCREEN = "SCREEN";

    /// Just for use gl sources before screen` egl initialize.
    AlEgl *aBaseCtx = nullptr;
    AlAbsContext *context = nullptr;
    std::vector<AlImageLayerModel *> mLayers;
    AlIdentityCreator mLayerIdCreator;
    std::mutex mLayerMtx;
    AlSize mWinSize;
    AlSize mCanvasSize;
    AlUCanvas::OnSaveListener onSaveListener = nullptr;
};


#endif //HWVC_ANDROID_ALIMAGEPROCESSOR_H
