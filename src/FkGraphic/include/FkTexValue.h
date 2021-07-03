/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKTEXVALUE_H
#define FK_GRAPHIC_FKTEXVALUE_H

#include "FkProgramValue.h"
#include "FkGraphicTexture.h"

FK_CLASS FkTexValue FK_EXTEND FkProgramValue {
public:
    FkTexValue();

    FkTexValue(const FkTexValue &o);

    virtual ~FkTexValue();

public:
    int32_t index = 0;
    std::shared_ptr<FkGraphicTexture> tex = nullptr;
};

#endif //FK_GRAPHIC_FKTEXVALUE_H