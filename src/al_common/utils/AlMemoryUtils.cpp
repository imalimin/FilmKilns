//
// Created by mingyi.li on 2019/10/22.
//

#include "AlMemoryUtils.h"
#include "arm_neon.h"

void AlMemoryUtils::rgba2bgra(uint8_t *data, int width, int height, int stride) {
#ifdef __ANDROID__
    long len = width * height * stride;
    int stride_bytes = stride * 16;
    long left = len % stride_bytes;
    len = len - left;
    for (int i = 0; i < len; i += stride_bytes) {
        uint8_t *target = data + i;
        uint8x16x4_t src = vld4q_u8(target);
        uint8x16x4_t dst;
        dst.val[0] = src.val[2];
        dst.val[1] = src.val[1];
        dst.val[2] = src.val[0];
        dst.val[3] = src.val[3];
        vst4q_u8(target, dst);
    }
    for (long i = len; i < len + left; i += stride) {
        std::swap(data[i], data[i + 2]);
    }
#else
    for (int i = 0; i < len; i += stride) {
        std::swap(data[i], data[i + 2]);
    }
#endif
}

size_t AlMemoryUtils::memcpy(uint8_t *dst, uint8_t *src, size_t size) {
#ifdef __ANDROID__
    long len = size;
    int stride_bytes = 64;
    long left = len % stride_bytes;
    len = len - left;
    for (int i = 0; i < len; i += stride_bytes) {
        uint8_t *target = src + i;
        uint8x16x4_t from = vld4q_u8(target);
        uint8x16x4_t to;
        to.val[0] = from.val[0];
        to.val[1] = from.val[1];
        to.val[2] = from.val[2];
        to.val[3] = from.val[3];
        vst4q_u8(dst + i, to);
    }
    std::memcpy(dst + len, src + len, left);
#else
    std::memcpy(dst, src, size);
#endif
}
