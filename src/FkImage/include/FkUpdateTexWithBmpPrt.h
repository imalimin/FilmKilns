/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKUPDATETEXWITHBMPPRT_H
#define FK_GRAPHIC_FKUPDATETEXWITHBMPPRT_H

#include "FkProtocol.h"
#include "FkBitmap.h"

FK_CLASS FkUpdateTexWithBmpPrt FK_EXTEND FkProtocol {
public:
    FkUpdateTexWithBmpPrt();

    FkUpdateTexWithBmpPrt(const FkUpdateTexWithBmpPrt &o);

    virtual ~FkUpdateTexWithBmpPrt();

public:
    FkID id = FK_ID_NONE;
    std::shared_ptr<FkBitmap> bmp = nullptr;
};

#endif //FK_GRAPHIC_FKUPDATETEXWITHBMPPRT_H