/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_RGBA2NV12FILTER_H
#define HWVC_ANDROID_RGBA2NV12FILTER_H

#include "Filter.h"

class RGBA2NV12Filter : public Filter {
public:
    RGBA2NV12Filter();

    ~RGBA2NV12Filter();

    bool init(int w, int h) override;

    virtual void draw(GLuint texture) override;

};


#endif //HWVC_ANDROID_RGBA2NV12FILTER_H
