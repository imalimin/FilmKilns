/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALCOLORSPACE_H
#define HWVC_ANDROID_ALCOLORSPACE_H

enum class AlColorSpace : int {
    NONE = -1,
    RGB,
    RGBA,
    YV12,      /** YUV420P. Called I420 on android. */
    NV12,      /** YUV420SP */
    GRAY,
};

#endif //HWVC_ANDROID_ALCOLORSPACE_H
