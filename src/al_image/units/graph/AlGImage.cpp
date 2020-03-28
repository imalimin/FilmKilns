/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlGImage.h"
#include "AlULayerWithOpt.h"
#include "AlULayerFilter.h"
#include "AlULayerDescriptor.h"
#include "AlUCanvas.h"

AlGImage::AlGImage(const std::string alias) : AlAbsGraph(alias) {
    registerAnUnit(new AlULayerWithOpt(ALIAS_OF_LAYER));
    registerAnUnit(new AlULayerFilter(ALIAS_OF_FILTER));
    registerAnUnit(new AlULayerDescriptor(ALIAS_OF_DESCRIPTOR));
    registerAnUnit(new AlUCanvas(ALIAS_OF_CANVAS));
}

AlGImage::~AlGImage() {

}
