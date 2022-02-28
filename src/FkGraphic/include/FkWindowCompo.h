/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-28 22:22:17
*/

#ifndef FK_GRAPHIC_FKWINDOWCOMPO_H
#define FK_GRAPHIC_FKWINDOWCOMPO_H

#include "FkComponent.h"
#include "FkGraphicWindow.h"

FK_CLASS FkWindowCompo FK_EXTEND FkComponent {
public:
    FkWindowCompo(std::shared_ptr<FkGraphicWindow> &_win);

    FkWindowCompo(const FkWindowCompo &o);

    virtual ~FkWindowCompo();

public:
    std::shared_ptr<FkGraphicWindow> win;
};

#endif //FK_GRAPHIC_FKWINDOWCOMPO_H