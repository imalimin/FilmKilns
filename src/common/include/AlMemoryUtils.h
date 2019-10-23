//
// Created by mingyi.li on 2019/10/22.
//

#ifndef HWVC_ANDROID_ALMEMORYUTILS_H
#define HWVC_ANDROID_ALMEMORYUTILS_H

#include "Object.h"
#include <cpu-features.h>

class AlMemoryUtils : public Object {
public:
    static size_t memcpy(const unsigned char *dst, const unsigned char *src, size_t size);

#ifdef __ANDROID__

    static bool supportNeon();

private:
    /**
     * Neon memory copy.
     */

    static size_t memcpy64(const unsigned char *dst, const unsigned char *src, size_t size);

    static size_t memcpy128(const unsigned char *dst, const unsigned char *src, size_t size);

#endif
};


#endif //HWVC_ANDROID_ALMEMORYUTILS_H
