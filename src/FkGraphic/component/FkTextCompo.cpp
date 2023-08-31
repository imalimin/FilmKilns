/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-08-31 13:01:23
*/

#include "FkTextCompo.h"

FK_IMPL_CLASS_TYPE(FkTextCompo, FkComponent)

FkTextCompo::FkTextCompo(const std::string &text,
                         int32_t textSize,
                         const FkColor &color,
                         const FkIntVec2 &pos)
        : FkComponent(), text(text), textSize(textSize), color(color), pos(pos) {

}

FkTextCompo::FkTextCompo(const FkTextCompo &o)
        : FkComponent(o), text(o.text), textSize(o.textSize), color(o.color), pos(o.pos) {

}

FkTextCompo::~FkTextCompo() {

}