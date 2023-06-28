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

FK_IMPL_CLASS_TYPE(FkRenderEngineCompo, FkComponent)

FkRenderEngineCompo::FkRenderEngineCompo(std::shared_ptr<FkEngine> &_engine)
        : FkComponent(), engine(_engine) {

}

FkRenderEngineCompo::FkRenderEngineCompo(const FkRenderEngineCompo &o)
        : FkComponent(o), engine(o.engine) {

}

FkRenderEngineCompo::~FkRenderEngineCompo() {
    engine = nullptr;
}