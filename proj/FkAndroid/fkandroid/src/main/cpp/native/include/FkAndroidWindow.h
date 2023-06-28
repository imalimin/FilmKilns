/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-23 14:15:54
*/

#ifndef FK_GRAPHIC_FKANDROIDWINDOW_H
#define FK_GRAPHIC_FKANDROIDWINDOW_H

#include "FkGraphicWindow.h"
#include "FkJavaRuntime.h"
#include <EGL/egl.h>

FK_SUPER_CLASS(FkAndroidWindow, FkGraphicWindow) {
FK_DEF_CLASS_TYPE_FUNC(FkAndroidWindow)

public:
    FkAndroidWindow(jobject surface);

    FkAndroidWindow(const FkAndroidWindow &o) = delete;

    virtual ~FkAndroidWindow();

    virtual void *getWindow() override;

private:
    void *winHandle = nullptr;
};

#endif //FK_GRAPHIC_FKANDROIDWINDOW_H