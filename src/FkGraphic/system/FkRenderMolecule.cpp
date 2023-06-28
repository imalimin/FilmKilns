/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkRenderMolecule.h"
#include "FkRenderModelAtom.h"
#include "FkRenderSourceAtom.h"
#include "FkRenderProcessAtom.h"
#include "FkRenderDeviceAtom.h"
#include "FkGLEnvAtom.h"
#include "FkRenderDefine.h"
#include "FkGLDefinition.h"
#include "FkRenderContext.h"

FK_IMPL_CLASS_TYPE(FkRenderMolecule, FkSimpleMolecule)

FkRenderMolecule::FkRenderMolecule() : FkSimpleMolecule() {

}

FkRenderMolecule::~FkRenderMolecule() {

}

void FkRenderMolecule::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
}

void FkRenderMolecule::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkGLEnvAtom>()
            ->next<FkRenderModelAtom>()
            ->next<FkRenderSourceAtom>()
            ->next<FkRenderProcessAtom>()
            ->next<FkRenderDeviceAtom>();
}

FkResult FkRenderMolecule::onCreate() {
    auto ret = FkSimpleMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    auto _context = std::dynamic_pointer_cast<FkRenderContext>(getContext());
    FkAssert(_context != nullptr, FK_NPE);
    GLint values[2];
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, values);
    _context->setMaxTextureSize(values[0]);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, values);
    _context->setMaxCountOfVertexTexture(values[0]);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, values);
    _context->setMaxCountOfFragmentTexture(values[0]);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, values);
    _context->setMaxViewportSize(values[0], values[1]);
    return ret;
}

FkResult FkRenderMolecule::onDestroy() {
    auto ret = FkSimpleMolecule::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderMolecule::onStart() {
    auto ret = FkSimpleMolecule::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkRenderMolecule::onStop() {
    auto ret = FkSimpleMolecule::onStop();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}
