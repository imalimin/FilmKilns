//
// Created by mingyi.li on 2021-01-31.
//

#include "FkGraphicTexComponent.h"

FkGraphicTexComponent::FkGraphicTexComponent() : FkGraphicComponent() {
    FK_MARK_SUPER
}

FkGraphicTexComponent::FkGraphicTexComponent(const FkGraphicTexComponent &o)
        : FkGraphicComponent(o) {
    FK_MARK_SUPER
}

FkGraphicTexComponent::~FkGraphicTexComponent() {
    texture = nullptr;
}
