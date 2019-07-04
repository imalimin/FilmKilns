/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWFRAMEFORMAT_H
#define HWVC_ANDROID_HWFRAMEFORMAT_H

enum class HwFrameFormat : int {
    HW_FMT_NONE = -1,
    HW_IMAGE_RGB,
    HW_IMAGE_RGBA,
    HW_IMAGE_YV12,     /** YUV420P. Called I420 on android. */
    HW_IMAGE_NV12,     /** YUV420SP */
    HW_IMAGE_END,    /** End flag of image formats. DO NOT USE if linking dynamically */

    HW_SAMPLE_U8 = 100,/** unsigned 8 bits */
    HW_SAMPLE_S16,     /** signed 16 bits */
    HW_SAMPLE_S32,     /** signed 32 bits */
    HW_SAMPLE_FLT,     /** float */
    HW_SAMPLE_DBL,     /** double */

    HW_SAMPLE_U8P,     /** unsigned 8 bits, planar */
    HW_SAMPLE_S16P,    /** signed 16 bits, planar */
    HW_SAMPLE_S32P,    /** signed 32 bits, planar */
    HW_SAMPLE_FLTP,    /** float, planar */
    HW_SAMPLE_DBLP,    /** double, planar */
    HW_SAMPLE_S64,     /** signed 64 bits */
    HW_SAMPLE_S64P,    /** signed 64 bits, planar */
    HW_SAMPLE_END,    /** End flag of sample formats. DO NOT USE if linking dynamically */
};

#endif //HWVC_ANDROID_HWFRAMEFORMAT_H
