/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALOPERATEFACTORY_H
#define HWVC_ANDROID_ALOPERATEFACTORY_H

#include "AlCropOperateModel.h"

al_class(AlOperateFactory) {
public:
    static AlCropOperateModel *crop(float left, float top, float right, float bottom);

private:
    AlOperateFactory() : Object() {};

    AlOperateFactory(const AlOperateFactory &o) : Object() {};

    ~AlOperateFactory() {};
};


#endif //HWVC_ANDROID_ALOPERATEFACTORY_H
