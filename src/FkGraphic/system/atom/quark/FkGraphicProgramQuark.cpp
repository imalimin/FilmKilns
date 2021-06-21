/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicProgramQuark.h"
#include "FkGraphicLayerPrt.h"
#include "FkGraphicProgramComponent.h"
#include "FkRenderRequestPrt.h"

FkGraphicProgramQuark::FkGraphicProgramQuark() : FkQuark() {
    FK_MARK_SUPER

}

FkGraphicProgramQuark::~FkGraphicProgramQuark() {

}

void FkGraphicProgramQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicProgramQuark::_onRenderRequest);
}

FkResult FkGraphicProgramQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkGraphicProgramAllocator>();
    return ret;
}

FkResult FkGraphicProgramQuark::onDestroy() {
    allocator->release();
    return FkQuark::onDestroy();
}

FkResult FkGraphicProgramQuark::onStart() {
    return FkQuark::onStart();
}

FkResult FkGraphicProgramQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkGraphicProgramQuark::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    auto prt = std::static_pointer_cast<FkGraphicLayerPrt>(p);
    auto comp = std::make_shared<FkGraphicProgramComponent>();
    FkProgramDescription desc(FkProgramDescription::kType::MATRIX);
    comp->program = allocator->alloc(desc);
    _fillValue(comp->program);
    prt->layer->addComponent(comp);
    return FK_OK;
}

FkResult FkGraphicProgramQuark::_fillValue(std::shared_ptr<FkGraphicProgram> program) {
    return FK_OK;
}
