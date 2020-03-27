/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALULAYERWITHOPT_H
#define HWVC_ANDROID_ALULAYERWITHOPT_H

#include "AlULayer.h"

al_class_ex(AlULayerWithOpt, AlULayer) {
public:
    AlULayerWithOpt(string alias);

    virtual ~AlULayerWithOpt();

    bool onOperatePaint(AlMessage *m);

    bool onOperateScale(AlMessage *m);

    bool onOperatePostScale(AlMessage *m);

    bool onOperateRotate(AlMessage *m);

    bool onOperatePostRotate(AlMessage *m);

    bool onOperateTrans(AlMessage *m);

    bool onOperatePostTrans(AlMessage *m);

    bool onOperateQuery(AlMessage *m);

    bool onOperateAlpha(AlMessage *m);

    bool onCropLayer(AlMessage *m);

    bool onCropLayerCancel(AlMessage *m);

    bool onCropCanvas(AlMessage *m);

};


#endif //HWVC_ANDROID_ALULAYERWITHOPT_H
