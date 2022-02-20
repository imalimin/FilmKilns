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

FK_CLASS FkEntity FK_EXTEND FkObject {
public:
    FkEntity();

    FkEntity(const FkEntity &o);

    virtual ~FkEntity();

    FkResult addComponent(std::shared_ptr<FkComponent> comp);

    FkResult findComponent(std::vector<std::shared_ptr<FkComponent>> &vec,
                           const FkClassType &classType);


    template<class T>
    std::shared_ptr<T> findComponent() {
        std::vector<std::shared_ptr<FkComponent>> vec;
        vec.clear();

        if (FK_OK != findComponent(vec, FkClassType::type<T>())) {
            return nullptr;
        }
        return Fk_POINTER_CAST(T, vec[0]);
    }

private:
    std::list<std::shared_ptr<FkComponent>> components;
};

#endif //FK_FRAMEWORK_FKENTITY_H