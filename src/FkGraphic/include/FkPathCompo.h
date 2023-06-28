/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-08 14:07:16
*/

#ifndef FK_GRAPHIC_FKPATHCOMPO_H
#define FK_GRAPHIC_FKPATHCOMPO_H

#include "FkComponent.h"
#include "FkPath.h"
#include "FkColor.h"
#include <mutex>

FK_SUPER_CLASS(FkPathCompo, FkComponent) {
FK_DEF_CLASS_TYPE_FUNC(FkPathCompo)

public:
    FkPathCompo(std::shared_ptr<FkPath> _path, FkColor color);

    FkPathCompo(const FkPathCompo &o);

    virtual ~FkPathCompo();

    void addPoint(int32_t x, int32_t y);

    void finish();

    size_t readPoints(std::vector<FkDoubleVec2> &_points);

    size_t getPoints(std::vector<FkDoubleVec2> &_points);

    int32_t getType();

    FkColor getColor();

    size_t size();

public:
    std::shared_ptr<FkPath> path = nullptr;
    FkColor color;

private:
    std::mutex mtx;
};

#endif //FK_GRAPHIC_FKPATHCOMPO_H