/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALATTRIBUTE_H
#define HWVC_ANDROID_ALATTRIBUTE_H

#include "Object.h"
#include <map>

al_class AlAttribute al_extend Object {
public:
    AlAttribute(std::string &k, std::string &v);

    AlAttribute(const AlAttribute &o);

    virtual ~AlAttribute();

    std::string name();

    std::string value();

private:
    std::pair<std::string, std::string> pair;
};


#endif //HWVC_ANDROID_ALATTRIBUTE_H
