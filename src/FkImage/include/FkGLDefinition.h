/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGLDEFINITION_H
#define FK_GRAPHIC_FKGLDEFINITION_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include "assert.h"

#define FK_DEF_GL_TAG "FilmKilnsGL"
#ifdef __FK_DEBUG__
#define FK_GL_CHECK(statement)                                        \
    do {                                                              \
        statement;                                                    \
        GLenum e = glGetError();                                      \
        if (e != GL_NO_ERROR) {                                       \
            FkLogE(FK_DEF_GL_TAG, "Get error(%i, 0x%x)", e, e);       \
            assert(e == GL_NO_ERROR);                                 \
        }                                                             \
    } while(0)
#else
#define FK_GL_CHECK(statement) statement
#endif

#endif //FK_GRAPHIC_FKGLDEFINITION_H