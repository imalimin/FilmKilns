/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-5-8 0:08:17
*/

#ifndef FK_GRAPHIC_FKCATMULLROMPATH_H
#define FK_GRAPHIC_FKCATMULLROMPATH_H

#include "FkPath.h"
#include <vector>

FK_SUPER_CLASS(FkCatmullRomPath, FkPath) {
FK_DEF_CLASS_TYPE_FUNC(FkCatmullRomPath)

public:
    FkCatmullRomPath(double _avgDistance);

    FkCatmullRomPath(const FkCatmullRomPath &o);

    virtual ~FkCatmullRomPath();

    virtual void addPoint(FkDoubleVec2 &point) override;

    virtual size_t getPoints(std::vector<FkDoubleVec2> &_points) override;

    virtual size_t readPoints(std::vector<FkDoubleVec2> &_points) override;

    virtual size_t size() override;

    virtual void finish() override;

private:
    std::vector<FkDoubleVec2> src;
    std::vector<FkDoubleVec2> points;
    double avgDistance;
};

#endif //FK_GRAPHIC_FKCATMULLROMPATH_H