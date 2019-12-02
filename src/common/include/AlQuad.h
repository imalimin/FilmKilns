/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALQUAD_H
#define HWVC_ANDROID_ALQUAD_H

#include "Object.h"
#include "AlPointF.h"

al_class(AlQuad) {
public:
    AlQuad();

    AlQuad(const AlQuad &o);

    virtual ~AlQuad();

    void setLeftTop(float x, float y);

    void setLeftBottom(float x, float y);

    void setRightBottom(float x, float y);

    void setRightTop(float x, float y);

    AlPointF leftTop();

    AlPointF leftBottom();

    AlPointF rightBottom();

    AlPointF rightTop();

    /// 碰撞检测
    /// \param pointF 进行检测的点
    /// \return true or fale
    bool contain(AlPointF pointF);

    /// 绕Y轴翻转
    /// \return
    AlQuad &mirrorVertical();

    AlQuad &mirrorHorizontal();

private:
    AlPointF _leftTop;
    AlPointF _leftBottom;
    AlPointF _rightBottom;
    AlPointF _rightTop;
};


#endif //HWVC_ANDROID_ALQUAD_H
