/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALULAYER_H
#define HWVC_ANDROID_ALULAYER_H

#include "Unit.h"
#include "HwAbsTexture.h"
#include "AlImageLayerManager.h"
#include "Al2DCoordinate.h"
#include "AlNV12ToRGBAFilter.h"
#include <deque>

al_def_unit(AlULayer, Unit) {
public:
    AlULayer(string alias);

    virtual ~AlULayer();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

private:
    /// 添加图层
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | layer model  | none      | none      |  path     |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool onAddLayer(AlMessage *msg);

    /// 添加纹理图层
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | size         | none      | none      |  path     |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool onAddLayerEmpty(AlMessage *msg);

    /// 解码图片通知
    /// FORMAT:
    /// +-----------------------------------------------------+
    /// | msg::obj        | msg::arg1 | msg::arg2 | msg::desc |
    /// +-----------------------------------------------------+
    /// | AbsTexture wrap | req code  | none      |  path     |
    /// +-----------------------------------------------------+
    /// \param msg
    /// \return
    bool onReceiveImage(AlMessage *msg);

    /// 删除图层
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | layer id  | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onRemoveLayer(AlMessage *msg);

    /// 移动图层层级
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | layer id  | layer idx |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onMoveLayer(AlMessage *msg);

    /// 重绘所有图层，并绘制到窗口
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onInvalidate(AlMessage *m);

    /// 把所有图层清除，并导入.qua文件
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | none         | none      | none      | .alx file |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool onImport(AlMessage *m);

    /// 导出为.qua文件
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | none         | none      | none      | .alx file |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool onExport(AlMessage *msg);

    bool onRedo(AlMessage *m);

    bool onUndo(AlMessage *m);

    /// 裁剪画布，并保持图层位置不变
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | AlOperateCrop| none      | none      |           |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool onCropCanvas(AlMessage *m);

    /// 调整画布，并保持图层位置不变
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | AlRectLoc    | width     | height    |           |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool onResizeCanvas(AlMessage *m);

    /// 查询所有图层信息
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | none         | none      | none      |           |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool onQueryInfo(AlMessage *msg);

    /// 更新纹理
    /// FORMAT:
    /// +-------------------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |  msg:sp  |
    /// +-------------------------------------------------------------+
    /// | buffer       | layer id  | none      |  none     |  size    |
    /// +-------------------------------------------------------------+
    /// \param msg
    /// \return
    bool _onUpdateLayerWithYUV(AlMessage *msg);

    bool _onUpdateLayerWithRGBA(AlMessage *msg);

    /// 清除图层
    /// FORMAT:
    /// +-------------------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |  msg:sp  |
    /// +-------------------------------------------------------------+
    /// | none         | layer id  | none      |  none     |  none    |
    /// +-------------------------------------------------------------+
    /// \param msg
    /// \return
    bool _onUpdateLayerClear(AlMessage *msg);

protected:
    void invalidate();

    bool _onCanvasUpdate(AlMessage *msg);

    bool _onWindowUpdate(AlMessage *msg);

    /// 查找Layer model
    /// \param x 屏幕坐标
    /// \param y 屏幕坐标
    /// \return
    AlImageLayerModel *findLayerModel(float x, float y);

    /// 查找Layer model
    /// \param layerId 图层id
    /// \return
    AlImageLayerModel *findLayerModel(int32_t layerId);

    /// 屏幕坐标装换到图层坐标
    /// \param model Layer model
    /// \param x 屏幕归一化坐标[-1,1]
    /// \param y 屏幕归一化坐标[-1,1]
    /// \return
    AlVec2 transWin2Layer(AlImageLayerModel *model, float x, float y);

    /// 屏幕坐标装换到画布坐标
    /// \param x 屏幕归一化坐标[-1,1]
    /// \param y 屏幕归一化坐标[-1,1]
    /// \return
    AlVec2 transWin2Canvas(float x, float y);

    AlSize getCanvasSize();

private:
    void _notifyAll(int32_t flags = 0);

    /// flags:
    /// +------------------------------------------------------------+
    /// | index     | value 0                 | value 1              |
    /// +------------------------------------------------------------+
    /// | 0         | none                    | Render to screen     |
    /// +------------------------------------------------------------+
    void _notifyFilter(AlImageLayer *layer, AlImageLayerModel *model, int32_t flags);

    void _saveStep();

    void _updateCoordination();

    void _cropCanvasAndKeepLoc(AlSize &src, AlSize &dst, AlPointF &anchor);

private:
    AlImageLayerManager mLayerManager;
    std::deque<std::string *> steps;
    Al2DCoordinate mWinCoord;
    Al2DCoordinate mCanvasCoord;
    ///For import cache.
    std::deque<AlImageLayerModel *> mImportQueue;
    HwAbsFBObject *fbo = nullptr;

    HwAbsTexture *y = nullptr, *u = nullptr, *v = nullptr;
    AlNV12ToRGBAFilter *yv12Filter = nullptr;
};


#endif //HWVC_ANDROID_ALULAYER_H
