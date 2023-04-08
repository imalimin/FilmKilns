/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-04-08 21:53:23
*/

#ifndef FK_BASE_FKTIMECOSTMEASURER_H
#define FK_BASE_FKTIMECOSTMEASURER_H

#include "FkObject.h"

FK_SUPER_CLASS(FkTimeCostMeasurer, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkTimeCostMeasurer)

public:
    FkTimeCostMeasurer(int32_t _hz);

    FkTimeCostMeasurer(const FkTimeCostMeasurer &o) = delete;

    virtual ~FkTimeCostMeasurer();

    void begin();

    void end(std::string tag);

    void reset();

    int64_t getAvgCostInUS();

private:
    void _print(std::string &tag);

private:
    int32_t _hz = 1000;
    int64_t _begin = 0;
    int64_t count = 0;
    int64_t time = 0;
};

#endif //FK_BASE_FKTIMECOSTMEASURER_H