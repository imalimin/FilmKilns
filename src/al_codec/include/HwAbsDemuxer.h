/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWABSDEMUXER_H
#define HWVC_ANDROID_HWABSDEMUXER_H

#include "Object.h"
#include "HwResult.h"

class HwAbsDemuxer : public Object {
public:
    HwAbsDemuxer();

    virtual ~HwAbsDemuxer();

    virtual HwResult configure(string filePath, string type);

    virtual HwResult start()=0;

protected:
    string filePath;
    string type;
};


#endif //HWVC_ANDROID_HWABSDEMUXER_H
