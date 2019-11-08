/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwWindow.h"

HwWindow::HwWindow() {

}

HwWindow::~HwWindow() {
    this->win = nullptr;
    this->width = 0;
    this->height = 0;
}

NativeWindowType HwWindow::getANativeWindow() {
    return win;
}

void HwWindow::setANativeWindow(NativeWindowType win) {
    this->win = win;
}

void HwWindow::setWidth(int width) { this->width = width; }

int HwWindow::getWidth() { return width; }

void HwWindow::setHeight(int height) { this->height = height; }

int HwWindow::getHeight() { return height; }