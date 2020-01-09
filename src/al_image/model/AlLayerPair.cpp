/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlLayerPair.h"

AlLayerPair::AlLayerPair(AlImageLayer *layer, AlImageLayerModel *model)
        : Object(),
          layer(layer),
          model(model) {

}

AlLayerPair::AlLayerPair(const AlLayerPair &o)
        : Object(),
          layer(o.layer),
          model(o.model) {

}

AlLayerPair::~AlLayerPair() {
    layer = nullptr;
    model = nullptr;
}
