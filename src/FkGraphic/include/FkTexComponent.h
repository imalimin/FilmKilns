/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKTEXCOMPONENT_H
#define FK_GRAPHIC_FKTEXCOMPONENT_H

#include "FkBufferComponent.h"

FK_CLASS FkTexComponent FK_EXTEND FkBufferComponent {
public:
    FkTexComponent();

    FkTexComponent(const FkTexComponent &o);

    virtual ~FkTexComponent();

};


#endif //FK_GRAPHIC_FKTEXCOMPONENT_H
