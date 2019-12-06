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

    /// 表示图层有更新，比如新增或删除图层
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onUpdateLayer(Message *msg);

    /// 重绘所有图层，并绘制到窗口
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onInvalidate(Message *m);

    /// 更新画布
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onUpdateCanvas(Message *m);

    /// 保存图片
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
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
