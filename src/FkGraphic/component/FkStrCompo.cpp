/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-12 9:37:07
*/

#include "FkStrCompo.h"

FkStrCompo::FkStrCompo(std::string &str) : FkComponent(), str(str) {
    FK_MARK_SUPER
}

FkStrCompo::FkStrCompo(const FkStrCompo &o) : FkComponent(o), str(o.str) {
    FK_MARK_SUPER
}

FkStrCompo::~FkStrCompo() {

}