/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_ALEXIF_H
#define HWVC_ANDROID_ALEXIF_H

#include "Object.h"

/// Exif info.
/// https://exiftool.org/TagNames/EXIF.html
al_class(AlExif) {
private:
    AlExif() : Object() {};

    AlExif(const AlExif &o) : Object() {}

    ~AlExif() {};

public:
    /// Orientation
    static constexpr int16_t EXIF_ORIENTATION_HORIZONTAL = 1;
    static constexpr int16_t EXIF_ORIENTATION_MIRROR_HORIZONTAL = 2;
    static constexpr int16_t EXIF_ORIENTATION_180 = 3;
    static constexpr int16_t EXIF_ORIENTATION_MIRROR_VERTICAL = 4;
    static constexpr int16_t EXIF_ORIENTATION_MIRROR_HORIZONTAL_270CW = 5;
    static constexpr int16_t EXIF_ORIENTATION_90CW = 6;
    static constexpr int16_t EXIF_ORIENTATION_MIRROR_HORIZONTAL_90CW = 7;
    static constexpr int16_t EXIF_ORIENTATION_270CW = 8;
};


#endif //HWVC_ANDROID_ALEXIF_H
