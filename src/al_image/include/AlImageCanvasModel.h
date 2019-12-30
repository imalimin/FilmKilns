/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGECANVASMODEL_H
#define HWVC_ANDROID_ALIMAGECANVASMODEL_H

#include "Object.h"
#include "AlAbsElemented.h"

al_class_ex(AlImageCanvasModel, AlAbsElemented) {
public:
    AlImageCanvasModel();

    AlImageCanvasModel(const AlImageCanvasModel &e);

    ~AlImageCanvasModel();

    void setSize(int32_t width, int32_t height);

    void set(int32_t width, int32_t height, int32_t color);

    int32_t getWidth();

    int32_t getHeight();

    int32_t getColor();

    virtual HwResult fromElement(AlElement *element) override;

    virtual HwResult toElement(AlElement **element) override;

private:
    int32_t width = 0;
    int32_t height = 0;
    int32_t color = 0;
};


#endif //HWVC_ANDROID_ALIMAGECANVASMODEL_H
