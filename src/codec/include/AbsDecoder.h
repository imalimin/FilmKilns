/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_ABSDECODER_H
#define HARDWAREVIDEOCODEC_ABSDECODER_H

#include "Object.h"
#include <string>
#include <HwResult.h>

using namespace std;

class AbsDecoder : public Object {
public:
    AbsDecoder();

    virtual ~AbsDecoder();

    virtual bool prepare(string path)=0;

    virtual void seek(int64_t us)=0;
};
namespace Hw {
    const HwResult MEDIA_WAIT = HwResult(-1);
    const HwResult MEDIA_EOF = HwResult(-541478725);
    const HwResult MEDIA_SUCCESS = Hw::SUCCESS;
}


#endif //HARDWAREVIDEOCODEC_ABSDECODER_H
