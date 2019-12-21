/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMGLAYERDESCRIPTION_H
#define HWVC_ANDROID_ALIMGLAYERDESCRIPTION_H

#include "AlImageLayerModel.h"
#include "AlSize.h"

/// 保存AlImageLayerModel的备份，并且附加了图层大小属性，不会拷贝AlAbsOperateModel
/// 用于送入AlAbsOperateModel进行测量兵改变图层属性，但不影响图层的原属性
al_class_ex(AlImgLayerDescription, AlImageLayerModel) {
public:
    AlImgLayerDescription(const AlImgLayerDescription &o);

    AlImgLayerDescription(const AlImageLayerModel &o);

    virtual ~AlImgLayerDescription();

    void setSize(AlSize size);

    AlSize getSize();

private:
    AlSize size;
};


#endif //HWVC_ANDROID_ALIMGLAYERDESCRIPTION_H
