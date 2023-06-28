/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-04 14:33:27
*/

#ifndef FK_IMAGE_FKPBMODEL_H
#define FK_IMAGE_FKPBMODEL_H

#include "FkModelInterface.h"
#include "FkPictureModel.pb.h"

using namespace com::alimin::fk;

FK_SUPER_CLASS(FkPbModel, FkModelInterface) {
FK_DEF_CLASS_TYPE_FUNC(FkPbModel)

public:
    FkPbModel(std::shared_ptr<pb::FkPictureModel> &model, const pb::FkImageLayer &pbLayer);

    FkPbModel(const FkPbModel &o) = delete;

    virtual ~FkPbModel();

    virtual FkID getLayerID() override;

    virtual FkDoubleVec2 getTranslate() override;

    virtual FkRational getRotation() override;

    virtual FkFloatVec3 getScale() override;

    virtual size_t getPaintPaths(std::vector<std::shared_ptr<FkPath>> &paths,
                                 std::vector<std::shared_ptr<FkPaint>> &paints) override;

private:
    std::shared_ptr<pb::FkPictureModel> model = nullptr;
    pb::FkImageLayer pbLayer;
};

#endif //FK_IMAGE_FKPBMODEL_H