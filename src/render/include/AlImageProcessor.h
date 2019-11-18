/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALIMAGEPROCESSOR_H
#define HWVC_ANDROID_ALIMAGEPROCESSOR_H

#include "HwAbsProcessor.h"
#include "HwResult.h"

class AlImageProcessor : public HwAbsProcessor {
public:
    AlImageProcessor();

    ~AlImageProcessor();

    void setCanvas(int32_t w, int32_t h, int32_t color);

    HwResult addLayer(const char *path);
};


#endif //HWVC_ANDROID_ALIMAGEPROCESSOR_H
