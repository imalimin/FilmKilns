/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICWINDOW_H
#define FK_GRAPHIC_FKGRAPHICWINDOW_H

#include "FkObject.h"
#include "FkSize.h"

#ifdef __ANDROID__

#include <EGL/egl.h>

#endif

FK_CLASS FkGraphicWindow FK_EXTEND FkObject {
public:
    FkGraphicWindow(NativeWindowType win, int width, int height);

    FkGraphicWindow(const FkGraphicWindow &o) = delete;

    virtual ~FkGraphicWindow();

    virtual NativeWindowType getNativeWindow();

    FkSize &getSize();

private:
    NativeWindowType win = nullptr;
    FkSize size;
};


#endif //FK_GRAPHIC_FKGRAPHICWINDOW_H
