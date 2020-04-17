//
// Created by mingyi.li on 2019/10/22.
//

#ifndef HWVC_ANDROID_ALMEMORYUTILS_H
#define HWVC_ANDROID_ALMEMORYUTILS_H

#include "Object.h"

class AlMemoryUtils : public Object {
public:
    static void rgba2bgra(uint8_t *data, int width, int height, int stride);

    static size_t memcpy(uint8_t *dst, uint8_t *src, size_t size);
};


#endif //HWVC_ANDROID_ALMEMORYUTILS_H
