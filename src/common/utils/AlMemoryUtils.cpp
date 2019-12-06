//
// Created by mingyi.li on 2019/10/22.
//

#include "../include/AlMemoryUtils.h"

size_t AlMemoryUtils::memcpy(const unsigned char *dst, const unsigned char *src, size_t size) {
#ifdef __ANDROID__
    if (AlMemoryUtils::supportNeon()) {
        if (size & 63) {
            int alignSize = (size - size % 64);
            memcpy64(dst, src, alignSize);
            std::memcpy((void *) (dst + alignSize), src + alignSize, size - alignSize);
        } else {
            memcpy64(dst, src, size);
        }
    } else {
        std::memcpy((void *) dst, src, size);
    }
    return size;
#else
    return memcpy(dst, src, size);
#endif
}

#ifdef __ANDROID__

bool AlMemoryUtils::supportNeon() {
    return android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM &&
           (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
}

size_t AlMemoryUtils::memcpy64(const unsigned char *dst, const unsigned char *src, size_t size) {
#if defined(__LP64__)
    asm volatile (
        "1:\n"
        "ld1 {v0.8b,v1.8b,v2.8b,v3.8b}, [%0], #32\n"  // load 32
        "subs %w2, %w2, #32\n"                        // 32 processed per loop
        "st1 {v0.8b,v1.8b,v2.8b,v3.8b}, [%1], #32\n"  // store 32
        "b.gt 1b\n"
        : "+r"(src),                                  // %0
        "+r"(dst),                                    // %1
        "+r"(size)
        :: "cc", "memory", "v0", "v1", "v2", "v3"     // Clobber List
        );
#elif defined(_ARM_ARCH_7)
    asm volatile (
    "1:\n"
    "VLDM %1!, {d0-d7}\n"
    "VSTM %0!, {d0-d7}\n"
    "SUBS %2, %2, #0x40\n"
    "BGT 1b\n"
    : "+r"(dst),
    "+r"(src),
    "+r"(size)
    ::"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "cc", "memory"
    );
#endif
    return size;
}

size_t AlMemoryUtils::memcpy128(const unsigned char *dst, const unsigned char *src, size_t size) {
#if defined(__LP64__)
    asm volatile (
        "1:\n"
        "ld1 {v0.2D,v1.2D,v2.2D,v3.2D}, [%0], #64\n"  // load 64
        "subs %w2, %w2, #64\n"                        // 64 processed per loop
        "st1 {v0.2D,v1.2D,v2.2D,v3.2D}, [%1], #64\n"  // store 64
        "b.gt 1b\n"
        : "+r"(src),                                  // %0
        "+r"(dst),                                    // %1
        "+r"(size)
        :: "cc", "memory", "v0", "v1", "v2", "v3"     // Clobber List
        );
#elif defined(_ARM_ARCH_7)
    asm volatile (
    "1:\n"
    "VLDM %0!,    {d0-d15}\n\t"
    "VSTM %1!,     {d0-d15}\n\t"
    "subs       %2, %2, #128\n"
    "bgt        1b\n"
    :"+r"(src),
    "+r"(dst),
    "+r"(size)
    :
    : "cc", "memory", "q0", "q1", "q2", "q3", "q4",
    "q5", "q6", "q7"
    );
#endif
    return size;
}

#endif