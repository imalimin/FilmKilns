/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${Alimin}
* CREATE TIME: 2022-2-23 13:30:46
*/

#ifndef FK_FRAMEWORK_FKFRAMEWORKDEFINE_H
#define FK_FRAMEWORK_FKFRAMEWORKDEFINE_H

#define FK_CAST_NULLABLE_PTR(name, type, value, statement)\
std::shared_ptr<type> name = nullptr;\
do {\
    name = std::dynamic_pointer_cast<type>(value);\
    if (name == nullptr) {\
        statement;\
    }\
} while(false)\


#define FK_CAST_NULLABLE_PTR_RETURN_INT(name, type, value) FK_CAST_NULLABLE_PTR(name, type, value, return FK_NPE)

#endif //FK_FRAMEWORK_FKFRAMEWORKDEFINE_H