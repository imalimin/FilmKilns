/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALABSENCODER_H
#define HWVC_ANDROID_ALABSENCODER_H

#include "Object.h"
#include "HwResult.h"
#include "AlCodec.h"
#include "HwAbsMediaFrame.h"

using namespace std;

AL_CLASS AlAbsEncoder AL_EXTEND Object {
public:
    AL_ENUM kType : int {
        SOFT = 0,
        HARD,
        HARD_ENC_TEX
    };

    AL_CLASS Desc AL_EXTEND Object {
    public:
        Desc();

        Desc(AlCodec::kID id, kType type);

        Desc(const Desc &o);

        virtual ~Desc();

    public:
        AlCodec::kID aID;
        AlCodec::kID vID;
        kType type;
    };

public:
    AlAbsEncoder(const AlAbsEncoder::Desc &desc);

    virtual ~AlAbsEncoder();

    virtual HwResult write(HwAbsMediaFrame *frame) = 0;

    virtual bool stop() = 0;

    virtual void release() = 0;

    virtual AlAbsEncoder::Desc getCodecDesc();

private:
    AlAbsEncoder(const AlAbsEncoder &o) : Object() {};

private:
    AlAbsEncoder::Desc desc;
};


#endif //HWVC_ANDROID_ALABSENCODER_H
