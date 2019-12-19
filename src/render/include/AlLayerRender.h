/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALLAYERRENDER_H
#define HWVC_ANDROID_ALLAYERRENDER_H

#include "Unit.h"
#include "AlImageCanvas.h"
#include "AlImageCanvasModel.h"

class AlLayerRender : public Unit {
public:
    ///code, msg, path
    typedef function<void(int32_t, const char *, const char *)> OnSaveListener;
public:
    AlLayerRender(const string &alias);

    virtual ~AlLayerRender();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    /// 更新画布
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onUpdateCanvas(AlMessage *m);

    /// 清空画布
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | bool      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onClear(AlMessage *m);

    /// 绘制图层到画布上
    /// FORMAT:
    /// +------------------------------------------------+
    /// | msg::obj              | msg::arg1 | msg::arg2 |
    /// +------------------------------------------------+
    /// | AlImageLayerDrawModel | none      | none      |
    /// +------------------------------------------------+
    /// \param msg
    /// \return
    bool onDraw(AlMessage *m);

    /// 上屏
    /// FORMAT:
    /// +----------------------------------+
    /// | msg::obj | msg::arg1 | msg::arg2 |
    /// +----------------------------------+
    /// | none     | none      | none      |
    /// +----------------------------------+
    /// \param msg
    /// \return
    bool onShow(AlMessage *m);

    /// 保存图片到文件
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool onSave(AlMessage *m);

    void setOnSaveListener(OnSaveListener listener);

private:
    AlImageCanvasModel *_getCanvas();

    void _newDefaultCanvas(AlSize size);

    void _draw(AlImageLayerDrawModel *description);

private:
    TextureAllocator *texAllocator = nullptr;
    AlImageCanvas mCanvas;
    OnSaveListener onSaveListener = nullptr;
};


#endif //HWVC_ANDROID_ALLAYERRENDER_H
