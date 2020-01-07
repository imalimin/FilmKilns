/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAbsMFilterAction.h"

AlAbsMFilterAction::AlAbsMFilterAction(std::string type) : AlAbsMAction(type) {

}

AlAbsMFilterAction::~AlAbsMFilterAction() {

}

HwResult AlAbsMFilterAction::measure(AlImgLayerDescription &layer,
                                     AlImageLayerDrawModel *description) {
    return HwResult(0);
}
