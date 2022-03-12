/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-12 9:37:18
*/

#ifndef FK_GRAPHIC_FKFILEPATHCOMPO_H
#define FK_GRAPHIC_FKFILEPATHCOMPO_H

#include "FkStrCompo.h"

FK_CLASS FkFilePathCompo FK_EXTEND FkStrCompo {
public:
    FkFilePathCompo(std::string &str);

    FkFilePathCompo(const FkFilePathCompo &o);

    virtual ~FkFilePathCompo();
};

#endif //FK_GRAPHIC_FKFILEPATHCOMPO_H