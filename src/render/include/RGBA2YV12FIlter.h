/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_RGBA2YV12FILTER_H
#define HWVC_ANDROID_RGBA2YV12FILTER_H

#include "Filter.h"

class RGBA2YV12Filter : public Filter {
public:
    RGBA2YV12Filter();

    ~RGBA2YV12Filter();

    bool init(int w, int h) override;
};


#endif //HWVC_ANDROID_RGBA2YV12FILTER_H
