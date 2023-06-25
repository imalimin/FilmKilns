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

FK_IMPL_CLASS_TYPE(FkImageContext, FkEngineContext)

std::shared_ptr<FkImageContext> FkImageContext::wrap(std::shared_ptr<FkEngineContext> _context) {
    auto *context = new FkImageContext(_context);
    return std::shared_ptr<FkImageContext>(context);
}

FkImageContext::FkImageContext(std::shared_ptr<FkEngineContext> &_context)
        : FkEngineContext(), _context(_context) {

}

FkImageContext::~FkImageContext() {
    _context = nullptr;
}

std::shared_ptr<FkEngineSettings> FkImageContext::getEngineSettings() {
    return _context->getEngineSettings();
}

std::shared_ptr<FkRenderEngine> FkImageContext::getRenderEngine() {
    auto proto = FK_FIND_COMPO(_context, FkRenderEngineCompo);
    if (proto) {
        return std::dynamic_pointer_cast<FkRenderEngine>(proto->engine);
    }
    return nullptr;
}
