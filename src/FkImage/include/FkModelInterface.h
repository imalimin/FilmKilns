/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-04 14:28:35
*/

#ifndef FK_IMAGE_FKMODELINTERFACE_H
#define FK_IMAGE_FKMODELINTERFACE_H

#include "FkObject.h"
#include "FkIntVec2.h"
#include "FkRational.h"
#include "FkFloatVec3.h"
#include "FkDoubleVec2.h"
#include "FkPath.h"
#include "FkPaint.h"
#include <vector>

FK_SUPER_CLASS(FkModelInterface, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkModelInterface)

public:
    FkModelInterface();

    FkModelInterface(const FkModelInterface &o);

    virtual ~FkModelInterface();

    virtual FkID getLayerID() = 0;

    virtual FkIntVec2 getTranslate() = 0;

    virtual FkRational getRotation() = 0;

    virtual FkFloatVec3 getScale() = 0;

    virtual size_t getPaintPaths(std::vector<std::shared_ptr<FkPath>> &paths,
                                 std::vector<std::shared_ptr<FkPaint>> &paints) = 0;

};

#endif //FK_IMAGE_FKMODELINTERFACE_H