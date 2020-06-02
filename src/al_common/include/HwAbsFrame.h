/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_HWABSFRAME_H
#define HARDWAREVIDEOCODEC_HWABSFRAME_H

#include "Object.h"
#include "HwBuffer.h"

class HwAbsFrame : virtual public Object {
public:
    HwAbsFrame(size_t size);

    virtual ~HwAbsFrame();

    virtual uint8_t *data();

    virtual size_t size();

private:
    HwBuffer *buf = nullptr;
};


#endif //HARDWAREVIDEOCODEC_HWABSFRAME_H
