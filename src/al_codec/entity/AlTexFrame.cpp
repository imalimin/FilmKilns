/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlTexFrame.h"
#include "HwTexture.h"

AlTexFrame::AlTexFrame(HwAbsTexture *tex)
        : HwVideoFrame(nullptr, HwFrameFormat::HW_IMAGE_RGBA, tex->getWidth(), tex->getHeight()) {
    this->_tex = HwTexture::wrap(tex);
}

AlTexFrame::AlTexFrame(const AlTexFrame &o)
        : HwVideoFrame(nullptr, HwFrameFormat::HW_IMAGE_RGBA, 0, 0) {

}

AlTexFrame::~AlTexFrame() {
    delete _tex;
    _tex = nullptr;
}

HwAbsTexture *AlTexFrame::tex() {
    return _tex;
}
