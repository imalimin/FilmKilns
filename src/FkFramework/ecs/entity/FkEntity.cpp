/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 16:57:49
*/

#include "FkEntity.h"

FK_IMPL_CLASS_TYPE(FkEntity, FkObject)

FkEntity::FkEntity() : FkObject() {


}

FkEntity::FkEntity(const FkEntity &o) : FkObject() {

    for (auto &it : o.components) {
        components.emplace_back(it);
    }
}

FkEntity::~FkEntity() {
    components.clear();
}

FkResult FkEntity::addComponent(std::shared_ptr<FkComponent> comp) {
    components.emplace_back(comp);
    return FK_OK;
}

FkResult FkEntity::findComponents(std::vector<std::shared_ptr<FkComponent>> &vec,
                                        const FkClassType &classType) {
    for (auto & component : components) {
        if (component->getClassType().is(classType)) {
            vec.emplace_back(component);
        }
    }
    return vec.empty() ? FK_FAIL : FK_OK;
}