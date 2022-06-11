/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-2 21:20:27
*/

#include "FkImageContext.h"
#include "FkRenderEngineCompo.h"

FK_IMPL_CLASS_TYPE(FkImageContext, FkQuarkContext)

FkImageContext::FkImageContext(): FkQuarkContext() {

}

FkImageContext::~FkImageContext() {

}

std::shared_ptr<FkRenderEngine> FkImageContext::getRenderEngine() {
    auto proto = FK_FIND_COMPO(this, FkRenderEngineCompo);
    if (proto) {
        return std::dynamic_pointer_cast<FkRenderEngine>(proto->engine);
    }
    return nullptr;
}
