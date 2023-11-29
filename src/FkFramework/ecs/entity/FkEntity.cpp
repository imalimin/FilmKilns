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
    FkAssert(comp != nullptr, FK_NPE);
    components.emplace_back(comp);
    return FK_OK;
}

FkResult FkEntity::addComponents(std::vector<std::shared_ptr<FkComponent>> &vec) {
    for (auto &compo : vec) {
        addComponent(compo);
    }
    return FK_OK;
}

FkResult FkEntity::removeComponent(std::shared_ptr<FkComponent> comp) {
    FkAssert(comp != nullptr, FK_NPE);
    auto itr = std::find(components.begin(), components.end(), comp);
    if (itr != components.end()) {
        components.erase(itr);
        return FK_OK;
    }
    return FK_SOURCE_NOT_FOUND;
}

FkResult FkEntity::findComponents(std::vector<std::shared_ptr<FkComponent>> &vec,
                                        const FkClassType &classType) {
    for (auto & component : components) {
        FkAssert(component != nullptr, FK_FAIL);
        if (component->getClassType().is(classType)) {
            vec.emplace_back(component);
        }
    }
    return vec.empty() ? FK_FAIL : FK_OK;
}

FkResult FkEntity::clearComponents(const FkClassType &classType) {
    auto itr = components.begin();
    while (itr != components.end()) {
        if ((*itr)->getClassType().is(classType)) {
            itr = components.erase(itr);
        } else {
            ++itr;
        }
    }
//    components.erase(std::find_if(components.begin(), components.end(),
//                                  [&](const auto &it) {
//                                      return (it)->getClassType().is(classType);
//                                  }));
    return FK_OK;
}

size_t FkEntity::countOfComponents() {
    return components.size();
}