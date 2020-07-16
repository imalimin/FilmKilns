//
// Created by limin on 2020/7/15.
//

#ifndef HWVC_ANDROID_ALMEDIADEF_H
#define HWVC_ANDROID_ALMEDIADEF_H

#include "Object.h"

AL_CLASS AlMediaDef AL_EXTEND Object {
public:
    /// Same with AV_PKT_FLAG_KEY/AV_PKT_FLAG_CORRUPT...
    static constexpr int32_t FLAG_KEY = 0x0001;
    static constexpr int32_t FLAG_CORRUPT = 0x0002;
    static constexpr int32_t FLAG_DISCARD = 0x0004;
    static constexpr int32_t FLAG_TRUSTED = 0x0008;
    static constexpr int32_t FLAG_DISPOSABLE = 0x0010;
    static constexpr int32_t FLAG_CONFIG = 0x0020;

    static constexpr int32_t FLAG_SEEK_DONE = 0x40000000;
    static constexpr int32_t FLAG_EOF = 0x80000000;

};


#endif //HWVC_ANDROID_ALMEDIADEF_H
