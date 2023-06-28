/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicWindow.h"

FK_IMPL_CLASS_TYPE(FkGraphicWindow, FkObject)

FkGraphicWindow::FkGraphicWindow(int width, int height)
        : FkObject(), _size(width, height) {

}

FkGraphicWindow::~FkGraphicWindow() {

}

FkSize FkGraphicWindow::size() {
    return _size;
}
