/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkTexFrame.h"
#include "FkTexture.h"

FK_IMPL_CLASS_TYPE(FkTexFrame, FkVideoFrame)

FkTexFrame::FkTexFrame(HwAbsTexture *tex)
        : FkVideoFrame(nullptr, kFrameFormat::IMAGE_RGBA, tex->getWidth(), tex->getHeight()) {
    this->_tex = HwTexture::wrap(tex);
}

FkTexFrame::FkTexFrame(const FkTexFrame &o)
        : FkVideoFrame(nullptr, kFrameFormat::IMAGE_RGBA, 0, 0) {

}

FkTexFrame::~FkTexFrame() {
    delete _tex;
    _tex = nullptr;
}

HwAbsTexture *FkTexFrame::tex() {
    return _tex;
}
