/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:30:26
*/

#ifndef FK_GRAPHIC_FKTEXCOMPO_H
#define FK_GRAPHIC_FKTEXCOMPO_H

#include "FkComponent.h"
#include "FkGraphicTexture.h"

FK_CLASS FkTexCompo FK_EXTEND FkComponent {
public:
    FkTexCompo(std::shared_ptr<FkGraphicTexture> &tex);

    FkTexCompo(const FkTexCompo &o);

    virtual ~FkTexCompo();

public:
    std::shared_ptr<FkGraphicTexture> tex = nullptr;
    int32_t index = 0;
};

#endif //FK_GRAPHIC_FKTEXCOMPO_H