/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWABSMUXER_H
#define HWVC_ANDROID_HWABSMUXER_H

#include "Object.h"
#include "HwResult.h"
#include "AlCodec.h"
#include "HwPacket.h"

class HwAbsMuxer : public Object {
public:
    HwAbsMuxer();

    virtual ~HwAbsMuxer();

    virtual HwResult configure(string filePath, string type);

    virtual HwResult start()=0;

    /**
     * add an audio or video track.
     * @return audio or video track id
     */
    virtual int32_t addTrack(AlCodec *codec)=0;

    /**
     * @param track track id
     * @return Hw::SUCCESS for success
     */
    virtual HwResult write(int32_t track, HwPacket *pkt)=0;

protected:
    string filePath;
    string type;

public:
    static const string TYPE_MP4;
    static const int32_t TRACK_NONE;
};


#endif //HWVC_ANDROID_HWABSMUXER_H
