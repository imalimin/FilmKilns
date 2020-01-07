/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALLAYERACTIONFACTORY_H
#define HWVC_ANDROID_ALLAYERACTIONFACTORY_H

#include "AlAbsMAction.h"

al_class(AlLayerActionFactory) {
public:
    static AlAbsMAction *crop(float left, float top, float right, float bottom);

    static AlAbsMAction *alignCrop(AlRational &r);

    static AlAbsMAction *mosaic(const AlPointF &pointF);

private:
    AlLayerActionFactory() : Object() {};

    AlLayerActionFactory(const AlLayerActionFactory &o) : Object() {};

    ~AlLayerActionFactory() {};
};


#endif //HWVC_ANDROID_ALLAYERACTIONFACTORY_H
