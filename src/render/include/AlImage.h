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
#include "AlImageLayerManager.h"
#include "AlGLContext.h"

class AlImage : public Unit {
public:
    AlImage(string alias);

    virtual ~AlImage();

    bool onCreate(Message *msg) override;

    bool onDestroy(Message *msg) override;

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

private:
    void _notifyAll(int32_t flag = 0);

    void _notifyDescriptor(AlImageLayer *layer);

    AlGLContext *_getContext();

private:
    TextureAllocator *texAllocator = nullptr;
    /// +------------------+
    /// |     Model        |
    /// +------------------+
    std::vector<AlImageLayerModel *> *getLayers();

    AlImageLayerManager mLayerManager;
};


#endif //HWVC_ANDROID_ALIMAGE_H
