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
#include "FkRenderDefine.h"

FkRenderMolecule::FkRenderMolecule() : FkSimpleMolecule() {
    FK_MARK_SUPER
}

FkRenderMolecule::~FkRenderMolecule() {

}

void FkRenderMolecule::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderMolecule::_onRender);
    FK_PORT_DELIVERY(desc, FkGenIDProto, FkRenderMolecule);
    FK_PORT_DELIVERY(desc, FkNewTexProto, FkRenderMolecule);
}

void FkRenderMolecule::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkRenderModelAtom>()
            ->next<FkRenderSourceAtom>()
            ->next<FkRenderProcessAtom>()
            ->next<FkRenderDeviceAtom>();
}

FkResult FkRenderMolecule::onCreate() {
    auto ret = FkSimpleMolecule::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    context = std::make_shared<FkContextCompo>("Render");
    context->create();
    context->makeCurrent();
    return ret;
}

FkResult FkRenderMolecule::onDestroy() {
    auto ret = FkSimpleMolecule::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    context->destroy();
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

FkResult FkRenderMolecule::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    context->makeCurrent();
    proto->env->addComponent(context);
    return FK_OK;
}