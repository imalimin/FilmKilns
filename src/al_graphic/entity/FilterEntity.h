/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_FILTERENTITY_H
#define HARDWAREVIDEOCODEC_FILTERENTITY_H

#include "Object.h"
#include <map>
#include <string>

using namespace std;

class FilterEntity : public Object {
public:
    static FilterEntity *fromFile(std::string file);

public:
    int version = 1;
    string name;
    string vertex;
    string fragment;
    map<string, float> params;
    map<string, string> samplers;

    FilterEntity();

    ~FilterEntity();
};


#endif //HARDWAREVIDEOCODEC_FILTERENTITY_H
