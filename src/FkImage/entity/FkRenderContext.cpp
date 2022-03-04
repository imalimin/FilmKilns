/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-2 21:20:27
*/

#include "FkRenderContext.h"
#include "FkRenderEngineCompo.h"

std::shared_ptr<FkRenderContext> FkRenderContext::wrap(std::shared_ptr<FkQuarkContext> context) {
    return std::shared_ptr<FkRenderContext>(new FkRenderContext(context));
}

FkRenderContext::FkRenderContext(std::shared_ptr<FkQuarkContext> &_context)
        : FkEntity(), context(_context) {
    FK_MARK_SUPER
}

FkRenderContext::~FkRenderContext() {

}

std::shared_ptr<FkRenderEngine> FkRenderContext::getRenderEngine() {
    if (context) {
        auto proto = context->findComponent<FkRenderEngineCompo>();
        if (proto) {
            return std::dynamic_pointer_cast<FkRenderEngine>(proto->engine);
        }
    }
    return nullptr;
}
