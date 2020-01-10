/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPOINTPATH_H
#define HWVC_ANDROID_ALPOINTPATH_H

#include "Object.h"
#include "AlVec2.h"
#include <vector>

al_class(AlPointPath) {
public:
    AlPointPath();

    AlPointPath(const AlPointPath &o);

    virtual ~AlPointPath();

    void paintTo(const AlVec2 &p);

    std::vector<float> *path();

private:
    std::vector<AlVec2> original;
    std::vector<float> _path;
};


#endif //HWVC_ANDROID_ALPOINTPATH_H
