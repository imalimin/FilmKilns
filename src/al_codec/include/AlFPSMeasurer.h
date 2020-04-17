/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALFPSMEASURER_H
#define HWVC_ANDROID_ALFPSMEASURER_H

#include "Object.h"

al_class(AlFPSMeasurer) {
public:
    AlFPSMeasurer();

    AlFPSMeasurer(const AlFPSMeasurer &o);

    virtual ~AlFPSMeasurer();

    void record(std::string tag = "");

    float fps();

private:
    float _fps = 0;
    int64_t count = 0;
    int64_t lastTime = 0;
    int64_t countOfTime = 0;
};


#endif //HWVCSCREEN_ALFPSMEASURER_H
