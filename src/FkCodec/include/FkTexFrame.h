/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef FK_CODEC_FKTEXFRAME_H
#define FK_CODEC_FKTEXFRAME_H

#include "FkVideoFrame.h"
#include "FkAbsTexture.h"

FK_SUPER_CLASS(FkTexFrame, FkVideoFrame) {
FK_DEF_CLASS_TYPE_FUNC(FkTexFrame)

public:
    FkTexFrame(HwAbsTexture *tex);

    virtual ~FkTexFrame();

    HwAbsTexture *tex();

private:
    FkTexFrame(const FkTexFrame &o);

private:
    HwAbsTexture *_tex = nullptr;
};


#endif //FK_CODEC_FKTEXFRAME_H
