/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKDEFINITION_H
#define FK_BASE_FKDEFINITION_H

#define FK_CLASS class
#define FK_ABS_CLASS class
#define FK_INTERFACE class
#define AFK_ENUM enum class
#define FK_EXTEND : public
#define FK_NULLABLE

#define FK_MARK_SUPER getClassType()->super(typeid(*this).name());
#define FK_CLASS_TYPE_EQUALS(src, target) (*((src).getClassType()) == *((target).getClassType()))

#endif //FK_BASE_FKDEFINITION_H
