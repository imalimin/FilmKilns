/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-10 12:58:54
*/

#ifndef FK_GRAPHIC_FKMESHPATH_H
#define FK_GRAPHIC_FKMESHPATH_H

#include "FkCatmullRomPath.h"

FK_SUPER_CLASS(FkMeshPath, FkCatmullRomPath) {
FK_DEF_CLASS_TYPE_FUNC(FkMeshPath)

public:
    FkMeshPath(float _strokeWidth, double _avgDistance);

    FkMeshPath(const FkMeshPath &o);

    virtual ~FkMeshPath();

    virtual void addPoint(FkDoubleVec2 &point) override;

    virtual size_t readPoints(std::vector<FkDoubleVec2> &_points) override;

    virtual size_t size() override;

private:
    float strokeWidth = 0.0f;
    std::vector<FkDoubleVec2> meshPoints;
    int readPos = 0;
};

#endif //FK_GRAPHIC_FKMESHPATH_H