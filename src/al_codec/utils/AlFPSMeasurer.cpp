/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFPSMeasurer.h"
#include "TimeUtils.h"
#include "AlLogcat.h"

#define TAG "AlFPSMeasurer"

AlFPSMeasurer::AlFPSMeasurer() : Object() {

}

AlFPSMeasurer::AlFPSMeasurer(const AlFPSMeasurer &o) : Object() {

}

AlFPSMeasurer::~AlFPSMeasurer() {

}

void AlFPSMeasurer::record(std::string tag) {
    int64_t time = TimeUtils::getCurrentTimeUS();
    if (lastTime > 0) {
        countOfTime += (time - lastTime);
        ++count;
        if (0 != countOfTime) {
            _fps = count * 1000000 / countOfTime;
        }
        if (count >= 100) {
            AlLogI(TAG, "%s fps %f", tag.c_str(), _fps);
            countOfTime = 0;
            count = 0;
        }
    }
    lastTime = time;
}

float AlFPSMeasurer::fps() {
    return _fps;
}
