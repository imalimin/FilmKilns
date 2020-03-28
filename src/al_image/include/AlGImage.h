/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALGIMAGE_H
#define HWVC_ANDROID_ALGIMAGE_H

#include "AlAbsGraph.h"

al_class_ex(AlGImage, AlAbsGraph) {
public:
    AlGImage(const std::string alias);

    virtual ~AlGImage();

private:
    const string ALIAS_OF_LAYER = "LAYER";
    const string ALIAS_OF_FILTER = "LAYER_FILTER";
    const string ALIAS_OF_DESCRIPTOR = "LAYER_DESCRIPTOR";
    const string ALIAS_OF_CANVAS = "LAYER_CANVAS";
};


#endif //HWVC_ANDROID_ALGIMAGE_H
