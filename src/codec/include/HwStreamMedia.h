/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HARDWAREVIDEOCODEC_HWSTREAMMEDIA_H
#define HARDWAREVIDEOCODEC_HWSTREAMMEDIA_H

#include "Unit.h"

class HwStreamMedia : public Unit {
public:
    HwStreamMedia(string alias);

    virtual ~HwStreamMedia();

    virtual bool onCreate(AlMessage *msg) = 0;

    virtual bool onDestroy(AlMessage *msg) = 0;

    virtual bool eventStart(AlMessage *msg) = 0;

    virtual bool eventPause(AlMessage *msg) = 0;

    virtual bool eventSeek(AlMessage *msg) = 0;

    virtual bool eventStop(AlMessage *msg) = 0;

    virtual bool eventSetSource(AlMessage *msg) = 0;

};


#endif //HARDWAREVIDEOCODEC_HWSTREAMMEDIA_H
