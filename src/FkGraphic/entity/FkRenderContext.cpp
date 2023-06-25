/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-06-11 11:07:20
*/

#include "FkRenderContext.h"
#include "FkGLDefinition.h"

FK_IMPL_CLASS_TYPE(FkRenderContext, FkEngineContext)

std::shared_ptr<FkRenderContext> FkRenderContext::wrap(std::shared_ptr<FkEngineContext> _context) {
    auto *context = new FkRenderContext(_context);
    return std::shared_ptr<FkRenderContext>(context);
}

FkRenderContext::FkRenderContext(std::shared_ptr<FkEngineContext> &_context)
        : FkEngineContext(), _context(_context) {

}

FkRenderContext::~FkRenderContext() {
    _context = nullptr;
}

std::shared_ptr<FkEngineSettings> FkRenderContext::getEngineSettings() {
    return _context->getEngineSettings();
}

std::shared_ptr<FkRenderEngineSettings> FkRenderContext::getRenderSettings() {
    return std::dynamic_pointer_cast<FkRenderEngineSettings>(_context->getEngineSettings());
}
