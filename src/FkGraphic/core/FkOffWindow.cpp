/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-5-1 20:21:49
*/

#include "FkOffWindow.h"

FK_IMPL_CLASS_TYPE(FkOffWindow, FkGraphicWindow)

FkOffWindow::FkOffWindow(int width, int height) : FkGraphicWindow(width, height) {

}

FkOffWindow::~FkOffWindow() {

}

void *FkOffWindow::getWindow() {
    return nullptr;
}
