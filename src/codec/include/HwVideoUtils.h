/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWVIDEOUTILS_H
#define HWVC_ANDROID_HWVIDEOUTILS_H

#include "Object.h"
#include "HwResult.h"
#include <vector>

class HwVideoUtils : public Object {
public:
    static HwResult
    remux(std::string input, std::string output,
          std::vector<int64_t> trimIns,
          std::vector<int64_t> trimOuts);

private:
    static bool
    contains(std::vector<int64_t> *trimIns, std::vector<int64_t> *trimOuts, int64_t pts);
};


#endif //HWVC_ANDROID_HWVIDEOUTILS_H
