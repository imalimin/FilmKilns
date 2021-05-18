//
// Created by Alimin on 2021/5/18.
//

#include "FkGraphicNewTexPtl.h"

FkGraphicNewTexPtl::FkGraphicNewTexPtl() : FkGraphicTexPtl() {
    FK_MARK_SUPER
}

FkGraphicNewTexPtl::FkGraphicNewTexPtl(const FkGraphicNewTexPtl &o) : FkGraphicTexPtl(o) {
    FK_MARK_SUPER

}

FkGraphicNewTexPtl::~FkGraphicNewTexPtl() {
    texId = FK_ID_NONE;
}