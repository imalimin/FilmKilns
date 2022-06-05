/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-04 14:28:35
*/

#include "FkModelInterface.h"

FK_IMPL_CLASS_TYPE(FkModelInterface, FkObject)

FkModelInterface::FkModelInterface() : FkObject() {

}

FkModelInterface::FkModelInterface(const FkModelInterface &o) : FkObject(o) {

}

FkModelInterface::~FkModelInterface() {

}