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
#include "AlImageLayerModel.h"
#include "AlMatrix.h"
#include <map>

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

    /// 删除指定的缓存layer
    /// FORMAT:
    /// +-----------------------------------------------+
    /// | msg::obj | msg::arg1  | msg::arg2 | msg::desc |
    /// +-----------------------------------------------+
    /// | none     |    id      | none      | none     |
    /// +-----------------------------------------------+
    /// \param msg
    /// \return
    bool onRemoveLayer(AlMessage *msg);

private:
    void _notifyDescriptor(AlImageLayer *layer, AlImageLayerModel *model, int32_t flags);

    /// 查找缓存的layer，如果不存在则重新拷贝一份
    /// 可能会拷贝失败，所以需要对返回的layer进行判空
    /// \param model
    /// \return
    AlImageLayer *_findLayer(AlImageLayerModel *model, AlImageLayer *layer);

    void _showDebugInfo(AlImageLayerModel *model, AlImageLayer *layer);

    void _translate(AlMatrix &mat, float transX, float transY, float &x, float &y);

    void _transWin2Layer(AlImageLayerModel *model, float &x, float &y);

    void _transCanvas2Layer(AlImageLayerModel *model, float &x, float &y);

private:
    std::map<int32_t, AlImageLayer *> layers;
    HwAbsFilter *paintFilter = nullptr;
    HwAbsFilter *copyFilter = nullptr;
};


#endif //HWVC_ANDROID_ALULAYERFILTER_H
