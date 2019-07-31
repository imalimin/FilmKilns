/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWABSMUXER_H
#define HWVC_ANDROID_HWABSMUXER_H

#include "Object.h"

class HwAbsMuxer : public Object {
public:
    HwAbsMuxer(string filePath, string type);

    virtual ~HwAbsMuxer();

    /**
     * @return audio track id
     */
    virtual int32_t addAudioTrack()=0;

    /**
     * @return video track id
     */
    virtual int32_t addVideoTrack()=0;

    /**
     * @param track track id
     * @return true for success
     */
    virtual bool write(int32_t track)=0;

protected:
    string filePath;
    string type;

public:
    static const string TYPE_MP4;
};


#endif //HWVC_ANDROID_HWABSMUXER_H
