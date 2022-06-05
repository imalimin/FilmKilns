/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-04 14:33:27
*/

#include "FkPbModel.h"
#include "FkMeshPath.h"

FK_IMPL_CLASS_TYPE(FkPbModel, FkModelInterface)

FkPbModel::FkPbModel(std::shared_ptr<pb::FkPictureModel> &model, const pb::FkImageLayer &pbLayer)
        : FkModelInterface(), model(model), pbLayer(pbLayer) {

}

FkPbModel::~FkPbModel() {

}

FkID FkPbModel::getLayerID() {
    return pbLayer.id();
}

FkIntVec2 FkPbModel::getTranslate() {
    return {pbLayer.trans().x(), pbLayer.trans().y()};
}

FkRational FkPbModel::getRotation() {
    return {pbLayer.rotation().num(), pbLayer.rotation().den()};
}

FkFloatVec3 FkPbModel::getScale() {
    return {pbLayer.scale().x(), pbLayer.scale().y(), 1.0f};
}

size_t FkPbModel::getPaintPaths(std::vector<std::shared_ptr<FkPath>> &paths,
                                std::vector<std::shared_ptr<FkPaint>> &paints) {
    auto size = pbLayer.paths_size();
    for (auto &pbPath: pbLayer.paths()) {
        std::shared_ptr<FkPaint> paint = std::make_shared<FkPaint>();
        paint->color = pbPath.color();
        std::shared_ptr<FkPath> path = nullptr;
        if (pbPath.type() == ((int32_t) FK_CLASS_OF(FkMeshPath)::type.getId())) {
            std::vector<FkDoubleVec2> meshPoints;
            for (auto &pbPoint: pbPath.points()) {
                meshPoints.emplace_back(pbPoint.x(), pbPoint.y());
            }
            path = std::make_shared<FkMeshPath>(meshPoints);
        }
        if (path != nullptr) {
            paths.emplace_back(path);
            paints.emplace_back(paint);
        }
    }
    return size;
}