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
#include <deque>

class AlULayer : public Unit {
public:
    typedef function<void(int32_t)> OnAlxLoadListener;
public:
    AlULayer(string alias);

    virtual ~AlULayer();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    /// 表示图层有更新，比如新增或删除图层
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onUpdateLayer(AlMessage *msg);

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

    bool onRedo(AlMessage *m);

    bool onUndo(AlMessage *m);

    bool onPaint(AlMessage *m);

    bool _onCanvasUpdate(AlMessage *msg);

    bool _onWindowUpdate(AlMessage *msg);

    void setOnAlxLoadListener(OnAlxLoadListener listener);

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

    AlVec2 _transWin2Layer(AlImageLayerModel *model, float x, float y);

private:
    /// +------------------+
    /// |     Model        |
    /// +------------------+
    std::vector<AlImageLayerModel *> *getLayers();

    AlImageLayerManager mLayerManager;
    std::deque<std::string *> steps;
    OnAlxLoadListener onAlxLoadListener = nullptr;
    Al2DCoordinate mWinCoord;
    Al2DCoordinate mCanvasCoord;
};


#endif //HWVC_ANDROID_ALULAYER_H
