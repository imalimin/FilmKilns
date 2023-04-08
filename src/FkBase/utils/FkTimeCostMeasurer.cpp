/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-04-08 21:53:23
*/

#include "FkTimeCostMeasurer.h"
#include "FkTimeUtils.h"

#define TAG "FkTimeCostMeasurer"

FK_IMPL_CLASS_TYPE(FkTimeCostMeasurer, FkObject)

FkTimeCostMeasurer::FkTimeCostMeasurer(int32_t hz) : FkObject(), _hz(hz) {

}

FkTimeCostMeasurer::~FkTimeCostMeasurer() {

}

void FkTimeCostMeasurer::begin() {
    _begin = FkTimeUtils::getCurrentTimeUS();
}

void FkTimeCostMeasurer::end(std::string tag) {
    ++count;
    time += FkTimeUtils::getCurrentTimeUS() - _begin;
    if (count >= _hz) {
        if (!tag.empty()) {
            _print(tag);
        }
        reset();
    }
}

void FkTimeCostMeasurer::reset() {
    count = 0;
    time = 0;
}

int64_t FkTimeCostMeasurer::getAvgCostInUS() {
    return time / count;
}

void FkTimeCostMeasurer::_print(std::string &tag) {
    FkLogI(TAG, "%s avg cost: %" PRId64 " US", tag.c_str(), getAvgCostInUS());
}
