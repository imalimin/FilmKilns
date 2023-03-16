/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-14 23:02:49
*/

#ifndef FK_GRAPHIC_FKDEVICEIMAGE_H
#define FK_GRAPHIC_FKDEVICEIMAGE_H

#include "FkObject.h"

FK_SUPER_CLASS(FkDeviceImage, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkDeviceImage)

public:
    FK_ENUM kType : int32_t {
        Default = 0,
        Android,
        IOS
    };

public:
    FkDeviceImage();

    FkDeviceImage(const FkDeviceImage &o);

    virtual ~FkDeviceImage();

    virtual kType type();

    virtual FkResult onCreate(void *arg0, int64_t arg1, int32_t arg2) = 0;

    virtual FkResult onDestroy() = 0;
};

#endif //FK_GRAPHIC_FKDEVICEIMAGE_H