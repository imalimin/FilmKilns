/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-26 13:23:41
*/

#ifndef FK_IMAGE_FKPICMODELBUILDER_H
#define FK_IMAGE_FKPICMODELBUILDER_H

#include "FkObject.h"
#include "FkPictureModel.pb.h"
#include "FkGraphicLayer.h"

using namespace com::alimin::fk;

FK_SUPER_CLASS(FkPicModelBuilder, FkObject),
        public std::enable_shared_from_this<FkPicModelBuilder> {
FK_DEF_CLASS_TYPE_FUNC(FkPicModelBuilder)

private:
    FkPicModelBuilder();

    FkPicModelBuilder(const FkPicModelBuilder &o) = delete;

public:
    virtual ~FkPicModelBuilder();

    static std::shared_ptr<FkPicModelBuilder> newBuilder();

    std::shared_ptr<FkPicModelBuilder> setCanvas(std::shared_ptr<FkGraphicLayer> &canvas);

    std::shared_ptr<FkPicModelBuilder> setLayer(std::shared_ptr<FkGraphicLayer> &layer);

    std::shared_ptr<pb::FkPictureModel> build();

private:
    void _newImageLayer(pb::FkImageLayer *pbLayer, std::shared_ptr<FkGraphicLayer> &layer);

    void _setMvpInfo(pb::FkImageLayer *pbLayer, std::shared_ptr<FkGraphicLayer> &layer);

private:
    std::shared_ptr<pb::FkPictureModel> model;
};

#endif //FK_IMAGE_FKPICMODELBUILDER_H