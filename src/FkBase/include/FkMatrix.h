/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKMATRIX_H
#define FK_BASE_FKMATRIX_H

#include "FkObject.h"
#include "glm.hpp"
#include "ext/matrix_clip_space.hpp"

FK_CLASS FkMatrix FK_EXTEND FkObject {
public:
    FkMatrix();

    FkMatrix(const FkMatrix &o);

    virtual ~FkMatrix();

    const void *get();

protected:
    glm::mat4 mat4;
};

#endif //FK_BASE_FKMATRIX_H