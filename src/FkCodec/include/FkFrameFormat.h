/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWFRAMEFORMAT_H
#define HWVC_ANDROID_HWFRAMEFORMAT_H

#include "FkColor.h"

/**
 * YUV420P: YV12(YYYYYYYY VV UU)/YU12/I420(YYYYYYYY UU VV)
 * YUV420SP: NV12(YYYYYYYY UVUV)/NV21(YYYYYYYY VUVU)
 */
FK_EM kFrameFormat : int {
    NONE = 0,
//    HW_IMAGE_RGB = static_cast<int>(AlColorFormat::RGB),
//    HW_IMAGE_RGBA = static_cast<int>(AlColorFormat::RGBA),
//    HW_IMAGE_YV12 = static_cast<int>(AlColorFormat::YV12),
//    HW_IMAGE_NV12 = static_cast<int>(AlColorFormat::NV12),
//    HW_IMAGE_END = static_cast<int>(AlColorFormat::END),    /** End flag of image formats. DO NOT USE if linking dynamically */

    SAMPLE_U8 = 100,/** unsigned 8 bits */
    SAMPLE_S16,     /** signed 16 bits */
    SAMPLE_S32,     /** signed 32 bits */
    SAMPLE_FLT,     /** float */
    SAMPLE_DBL,     /** double */

    SAMPLE_U8P,     /** unsigned 8 bits, planar */
    SAMPLE_S16P,    /** signed 16 bits, planar */
    SAMPLE_S32P,    /** signed 32 bits, planar */
    SAMPLE_FLTP,    /** float, planar */
    SAMPLE_DBLP,    /** double, planar */
    SAMPLE_S64,     /** signed 64 bits */
    SAMPLE_S64P,    /** signed 64 bits, planar */
    SAMPLE_END,    /** End flag of sample formats. DO NOT USE if linking dynamically */
};

#endif //HWVC_ANDROID_HWFRAMEFORMAT_H
