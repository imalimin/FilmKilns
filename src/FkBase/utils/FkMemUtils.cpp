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

FK_IMPL_CLASS_TYPE(FkMemUtils, FkObject)

FkResult FkMemUtils::copySubImage(std::shared_ptr<FkBuffer> &src, int32_t width,
                                  std::shared_ptr<FkBuffer> &dst, FkSize size,
                                  FkIntVec2 pos) {
    for (int y = 0; y < size.getHeight(); ++y) {
        auto srcIndex = ((pos.y + y) * width + pos.x) * 4;
        auto dstIndex = y * size.getWidth() * 4;
        auto count = size.getWidth() * 4;
        memcpy(dst->data() + dstIndex, src->data() + srcIndex, count);
    }
    return FK_OK;
}