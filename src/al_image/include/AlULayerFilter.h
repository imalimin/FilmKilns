/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALULAYERFILTER_H
#define HWVC_ANDROID_ALULAYERFILTER_H

#include "Unit.h"
#include "AlImageLayer.h"
#include "HwAbsFilter.h"
#include "AlTexAllocator.h"
#include "AlImageLayerModel.h"

al_class_ex(AlULayerFilter, Unit) {
public:
    AlULayerFilter(string alias);

    virtual ~AlULayerFilter();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    /// 进行filter action渲染操作
    /// FORMAT:
    /// +--------------------------------------------------------------+
    /// | msg::obj                | msg::arg1  | msg::arg2 | msg::desc |
    /// +--------------------------------------------------------------+
    /// | AlImageLayer ObjectBox | flags.      | none      | none     |
    /// +--------------------------------------------------------------+
    /// \param msg
    /// \return
    bool onDoFilterAction(AlMessage *msg);

private:
    void _notifyDescriptor(AlImageLayer *layer, AlImageLayerModel *model, int32_t flags);

private:
    AlTexAllocator *texAllocator = nullptr;
    HwAbsFilter *mosaicFilter = nullptr;
    AlImageLayer *nLayer = nullptr;
};


#endif //HWVC_ANDROID_ALULAYERFILTER_H
