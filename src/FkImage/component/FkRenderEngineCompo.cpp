/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-2 20:42:01
*/

#include "FkRenderEngineCompo.h"

FkRenderEngineCompo::FkRenderEngineCompo(std::shared_ptr<FkRenderEngine> &_engine)
        : FkComponent(), engine(_engine) {
    FK_MARK_SUPER
}

FkRenderEngineCompo::FkRenderEngineCompo(const FkRenderEngineCompo &o)
        : FkComponent(o), engine(o.engine) {
    FK_MARK_SUPER
}

FkRenderEngineCompo::~FkRenderEngineCompo() {
    engine = nullptr;
}