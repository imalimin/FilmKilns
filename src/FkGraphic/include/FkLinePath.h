/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-18 13:15:10
*/

#ifndef FK_GRAPHIC_FKLINEPATH_H
#define FK_GRAPHIC_FKLINEPATH_H

#include "FkPath.h"
#include "FkDoubleVec2.h"

FK_SUPER_CLASS(FkLinePath, FkPath) {
FK_DEF_CLASS_TYPE_FUNC(FkLinePath)

public:
    FkLinePath(double _avgDistance, float _pixelsOfSensitivity);

    FkLinePath(const FkLinePath &o);

    virtual ~FkLinePath();

    virtual void addPoint(FkDoubleVec2 &point) override;

    virtual size_t readPoints(std::vector<FkDoubleVec2> &_points) override;

    virtual size_t getPoints(std::vector<FkDoubleVec2> &_points) override;

    virtual size_t size() override;

    virtual void finish() override;

private:
    std::vector<FkDoubleVec2> src;
    std::vector<FkDoubleVec2> points;
    float pixelsOfSensitivity = 5.0f;
    double avgDistance;
    int readPos = 0;
};

#endif //FK_GRAPHIC_FKLINEPATH_H