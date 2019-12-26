/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_FILTERREADER_H
#define HARDWAREVIDEOCODEC_FILTERREADER_H

#include <iostream>
#include <string>
#include "Object.h"
#include "tinyxml.h"
#include "../entity/FilterEntity.h"

using namespace std;

class FilterReader : public Object {
public:
    FilterReader(char *path);

    virtual ~FilterReader();

    FilterEntity *read();

private:
    TiXmlDocument doc;
};


#endif //HARDWAREVIDEOCODEC_FILTERREADER_H
