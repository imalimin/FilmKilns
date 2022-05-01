/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-5-1 20:21:49
*/

#ifndef FK_GRAPHIC_FKOFFWINDOW_H
#define FK_GRAPHIC_FKOFFWINDOW_H

#include "FkGraphicWindow.h"

FK_SUPER_CLASS(FkOffWindow, FkGraphicWindow) {
FK_DEF_CLASS_TYPE_FUNC(FkOffWindow)

public:
    FkOffWindow(int width, int height);

    FkOffWindow(const FkOffWindow &o) = delete;

    virtual ~FkOffWindow();

    virtual void *getWindow() override;
};

#endif //FK_GRAPHIC_FKOFFWINDOW_H