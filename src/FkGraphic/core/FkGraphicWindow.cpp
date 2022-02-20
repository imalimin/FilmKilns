/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicWindow.h"

FkGraphicWindow::FkGraphicWindow(NativeWindowType win, int width, int height)
        : FkObject(), win(win), size(width, height) {
    FK_MARK_SUPER
}

FkGraphicWindow::~FkGraphicWindow() {

}

NativeWindowType FkGraphicWindow::getNativeWindow() {
    return win;
}

FkSize &FkGraphicWindow::getSize() {
    return size;
}
