/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-12 9:37:18
*/

#include "FkFilePathCompo.h"

FkFilePathCompo::FkFilePathCompo(std::string &str) : FkStrCompo(str) {
    FK_MARK_SUPER
}

FkFilePathCompo::FkFilePathCompo(const FkFilePathCompo &o) : FkStrCompo(o) {
    FK_MARK_SUPER
}

FkFilePathCompo::~FkFilePathCompo() {

}