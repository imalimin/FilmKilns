/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 13:47:11
*/

#include "FkRenderProgramQuark.h"
#include "FkRenderProto.h"
#include "FkRenderProgramCompo.h"
#include "FkPointVertexCompo.h"
#include "FkPathCompo.h"
#include "FkRenderContext.h"

FK_IMPL_CLASS_TYPE(FkRenderProgramQuark, FkQuark)

FkRenderProgramQuark::FkRenderProgramQuark() : FkQuark() {

}

FkRenderProgramQuark::~FkRenderProgramQuark() {

}

void FkRenderProgramQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderProgramQuark::_onRender);
}

FkResult FkRenderProgramQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkRenderProgramQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderProgramQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkGraphicProgramAllocator>(10);
    return ret;
}

FkResult FkRenderProgramQuark::onStop() {
    allocator->release();
    allocator = nullptr;
    return FkQuark::onStop();
}

FkResult FkRenderProgramQuark::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (FK_INSTANCE_OF(proto->device, FkBufDeviceEntity)) {
        return FK_SKIP;
    }

    auto pointCompo = FK_FIND_COMPO(proto->materials, FkPointVertexCompo);
    std::vector<std::shared_ptr<FkComponent>> paths;
    auto ret = proto->materials->findComponents(paths, FkPathCompo_Class::type);
    if (ret == FK_OK) {
        auto compo = std::make_shared<FkRenderProgramCompo>();
        FkProgramDescription desc(FkProgramDescription::kType::PATH);
        compo->program = allocator->alloc(desc);
        if (compo->program != nullptr) {
            proto->materials->addComponent(compo);
        }
    } else if (pointCompo != nullptr) {
        auto compo = std::make_shared<FkRenderProgramCompo>();
        FkProgramDescription desc(FkProgramDescription::kType::POINT);
        compo->program = allocator->alloc(desc);
        if (compo->program != nullptr) {
            proto->materials->addComponent(compo);
        }
    } else {
        auto compo = std::make_shared<FkRenderProgramCompo>();
        FkProgramDescription desc(FkProgramDescription::kType::MATRIX);
        auto context = std::dynamic_pointer_cast<FkRenderContext>(getContext());
        if (context) {
            desc.maxCountOfFragmentTexture = context->getMaxCountOfFragmentTexture();
        }
        if (FK_INSTANCE_OF(proto->device, FkScreenEntity)) {
            desc.type = FkProgramDescription::kType::MATRIX_WITH_CANVAS_BACKGROUND;
            desc.maxCountOfFragmentTexture = 1;
        }
        compo->program = allocator->alloc(desc);
        if (compo->program == nullptr) {
            return FK_SOURCE_NOT_FOUND;
        }
        proto->materials->addComponent(compo);
    }
    return FK_OK;
}