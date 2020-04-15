/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_HWRESULT_H
#define HARDWAREVIDEOCODEC_HWRESULT_H

#include "Object.h"

class HwResult : public Object {
public:
    int code;

    HwResult(int code);

    HwResult(const HwResult &value);

    ~HwResult();

    bool operator==(const HwResult &ret) const;

    bool operator!=(const HwResult &ret) const;

    bool operator==(int ret) const;

    bool operator!=(int ret) const;

    HwResult &operator=(const HwResult &value);
};

namespace Hw {
    const HwResult SUCCESS = HwResult(0);
    const HwResult FAILED = HwResult(-1);
}

#endif //HARDWAREVIDEOCODEC_HWRESULT_H
