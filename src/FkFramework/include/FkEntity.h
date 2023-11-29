/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-20 16:57:49
*/

#ifndef FK_FRAMEWORK_FKENTITY_H
#define FK_FRAMEWORK_FKENTITY_H

#include "FkObject.h"
#include "FkComponent.h"
#include <vector>
#include <list>

#define FK_FIND_COMPO(ENTITY, NAME) \
ENTITY->findComponent<NAME>(FK_CLASS_TYPE(NAME))                            \

FK_SUPER_CLASS(FkEntity, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkEntity)

public:
    FkEntity();

    FkEntity(const FkEntity &o);

    virtual ~FkEntity();

    FkResult addComponent(std::shared_ptr<FkComponent> comp);

    FkResult addComponents(std::vector<std::shared_ptr<FkComponent>> &vec);

    FkResult removeComponent(std::shared_ptr<FkComponent> comp);

    FkResult findComponents(std::vector<std::shared_ptr<FkComponent>> &vec,
                           const FkClassType &classType);

    FkResult clearComponents(const FkClassType &classType);

    size_t countOfComponents();

    template<class T>
    std::shared_ptr<T> findComponent(const FkClassType &classType) {
        std::vector<std::shared_ptr<FkComponent>> vec;
        if (FK_OK != findComponents(vec, classType)) {
            return nullptr;
        }
        return std::dynamic_pointer_cast<T>(vec[0]);
    }

    FkResult copyComponentFrom(std::shared_ptr<FkEntity> src, const FkClassType &classType) {
        std::vector<std::shared_ptr<FkComponent>> vec;
        if (FK_OK == src->findComponents(vec, classType)) {
            return addComponent(vec[0]);
        }
        return FK_SOURCE_NOT_FOUND;
    }

private:
    std::list<std::shared_ptr<FkComponent>> components;
};

#endif //FK_FRAMEWORK_FKENTITY_H