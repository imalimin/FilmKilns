/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-11 22:42:25
*/

#ifndef FK_BASE_FKMEMUTILS_H
#define FK_BASE_FKMEMUTILS_H

#include "FkObject.h"
#include "FkBuffer.h"
#include "FkSize.h"
#include "FkIntVec2.h"

FK_SUPER_CLASS(FkMemUtils, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkMemUtils)

public:
    static FkResult copySubImage(std::shared_ptr<FkBuffer> &src, int32_t width,
                                 std::shared_ptr<FkBuffer> &dst, FkSize size,
                                 FkIntVec2 pos);

private:
    FkMemUtils() {};

    FkMemUtils(const FkMemUtils &o) = delete;

public:
    virtual ~FkMemUtils() {};
};

#endif //FK_BASE_FKMEMUTILS_H