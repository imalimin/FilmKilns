/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-11 22:42:25
*/

#include "FkMemUtils.h"
#include <arm_neon.h>

//void __attribute__ ((noinline)) neon_memcpy1(unsigned char *dst, unsigned char *src, int sz) {
//#ifdef __ARM_NEON
//    asm(
//    "neoncopypld:\n"
//    "    vldm r0!,{d0-d7}\n"
//    "    vstm r1!,{d0-d7}\n"
//    "    subs r2,r2#0x40\n"
//    "    bge neoncopypld\n"
//    );
//#endif
//}

FK_IMPL_CLASS_TYPE(FkMemUtils, FkObject)

FkResult FkMemUtils::copySubImage(std::shared_ptr<FkBuffer> &src, int32_t width,
                                  std::shared_ptr<FkBuffer> &dst, FkSize size,
                                  FkIntVec2 pos) {
    for (int y = 0; y < size.getHeight(); ++y) {
        auto srcIndex = ((pos.y + y) * width + pos.x) * 4;
        auto dstIndex = y * size.getWidth() * 4;
        auto count = size.getWidth() * 4;
        std::memcpy(dst->data() + dstIndex, src->data() + srcIndex, count);
//        FkMemUtils::neon_memcpy(dst->data() + dstIndex, src->data() + srcIndex, count);
    }
    return FK_OK;
}

int FkMemUtils::memcpy(uint8_t *dst, uint8_t *src, int size) {
    int64_t i = 0;
    auto stride = 64;
    auto nSize = size / stride * stride;
    for (i = 0; i < nSize; i += stride) {
        uint8x16x4_t nSrc = vld4q_u8(src + i);
        uint8x16x4_t nDst;
        nDst.val[0] = nSrc.val[0];
        nDst.val[1] = nSrc.val[1];
        nDst.val[2] = nSrc.val[2];
        nDst.val[3] = nSrc.val[3];
        vst4q_u8(dst + i, nDst);
    }
    for (i = 0; i < size; ++i) {
        memcpy(dst + i, src + i, size - nSize);
    }
    return nSize;
}

void FkMemUtils::neon_memcpy(volatile unsigned char *dst,
                             volatile unsigned char *src,
                             int sz) {
    if (sz & 63)
        sz = (sz & -64) + 64;
#ifdef __ARM__
    asm volatile (
    "NEONCopyPLD: \n"
    " VLDM %[src]!,{d0-d7} \n"
    " VSTM %[dst]!,{d0-d7} \n"
    " SUBS %[sz],%[sz],#0x40 \n"
    " BGT NEONCopyPLD \n"
    : [dst]"+r"(dst), [src]"+r"(src), [sz]"+r"(sz) : : "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "cc", "memory"
    );
#endif
    asm volatile (
    "NEONCopyPLD: \n"
    "sub %[dst], %[dst], #64 \n"
    "1: \n"
    "ldnp q0, q1, [%[src]] \n"
    "ldnp q2, q3, [%[src], #32] \n"
    "add %[dst], %[dst], #64 \n"
    "subs %w[sz], %w[sz], #64 \n"
    "add %[src], %[src], #64 \n"
    "stnp q0, q1, [%[dst]] \n"
    "stnp q2, q3, [%[dst], #32] \n"
    "b.gt 1b \n"
    : [dst]"+r"(dst), [src]"+r"(src), [sz]"+r"(sz) : : "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "cc", "memory"
    );
}
