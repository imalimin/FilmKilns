/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUIMAGECODEC_H
#define HWVC_ANDROID_ALUIMAGECODEC_H

#include "Unit.h"
#include "AlRational.h"
#include "HwAbsTexture.h"

al_class_ex(AlUImageCodec, Unit) {
public:
    AlUImageCodec(const string &alias);

    virtual ~AlUImageCodec();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    /// 解码指定路径的图片
    /// FORMAT:
    /// +----------------------------------------------+
    /// | msg::obj | msg::arg1 | msg::arg2 | msg::desc |
    /// +----------------------------------------------+
    /// | none     | req code  |   none    |  path     |
    /// +----------------------------------------------+
    /// \param msg
    /// \return
    bool onDecode(AlMessage *msg);

    /// 保存图片到指定路径
    /// FORMAT:
    /// +----------------------------------------------+
    /// | msg::obj | msg::arg1 | msg::arg2 | msg::desc |
    /// +----------------------------------------------+
    /// | none     | action    |   none    |  path     |
    /// +----------------------------------------------+
    /// \param msg
    /// \return
    bool onEncode(AlMessage *msg);

private:
    void _correctAngle(HwAbsTexture **tex, AlRational radian);

    bool _read(HwAbsTexture *tex, AlBuffer *buf);

private:
    HwAbsFBObject *fbo = nullptr;
};


#endif //HWVC_ANDROID_ALUIMAGECODEC_H
