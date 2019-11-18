/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGECANVASMODEL_H
#define HWVC_ANDROID_ALIMAGECANVASMODEL_H

#include "Object.h"

al_class(AlImageCanvasModel) {
public:
    AlImageCanvasModel();

    AlImageCanvasModel(const AlImageCanvasModel &e);

    ~AlImageCanvasModel();

    void setSize(int32_t width, int32_t height);

    void set(int32_t width, int32_t height, int32_t color);

private:
    int32_t width = 0;
    int32_t height = 0;
    int32_t color = 0;
};


#endif //HWVC_ANDROID_ALIMAGECANVASMODEL_H
