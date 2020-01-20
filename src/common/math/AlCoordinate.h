/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCOORDINATE_H
#define HWVC_ANDROID_ALCOORDINATE_H

#include "Object.h"
#include "AlVec2.h"

al_class(AlCoordinate) {
public:
    static AlCoordinate &create();

    AlCoordinate(const AlCoordinate &o);

    ~AlCoordinate();

    void translate(AlVec2 vec2, AlCoordinate &coordinate);

private:
    AlCoordinate() : Object() {};
private:

};


#endif //HWVC_ANDROID_ALCOORDINATE_H
