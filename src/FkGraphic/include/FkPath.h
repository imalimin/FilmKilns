/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-5-7 23:55:26
*/

#ifndef FK_GRAPHIC_FKPATH_H
#define FK_GRAPHIC_FKPATH_H

#include "FkObject.h"
#include "FkDoubleVec2.h"

FK_SUPER_CLASS(FkPath, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkPath)

public:
    FkPath();

    FkPath(const FkPath &o);

    virtual ~FkPath();

    virtual void addPoint(FkDoubleVec2 &point) = 0;

    virtual size_t getPoints(std::vector<FkDoubleVec2> &_points) = 0;

    virtual size_t readPoints(std::vector<FkDoubleVec2> &_points) = 0;

    virtual size_t size() = 0;

    virtual void finish() = 0;
};

#endif //FK_GRAPHIC_FKPATH_H