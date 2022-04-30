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

FK_SUPER_CLASS(FkGraphicWindow, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicWindow)

public:
    FkGraphicWindow(int width, int height);

    FkGraphicWindow(const FkGraphicWindow &o) = delete;

    virtual ~FkGraphicWindow();

    virtual FkSize size();

    virtual void *getWindow() = 0;

protected:
    FkSize _size;
};


#endif //FK_GRAPHIC_FKGRAPHICWINDOW_H
