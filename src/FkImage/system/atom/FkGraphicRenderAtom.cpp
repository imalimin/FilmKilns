/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicRenderAtom.h"
#include "FkRenderRequestPrt.h"
#include "FkEmptyQuark.h"
#include "FkDeviceEntity.h"
#include "FkRenderContext.h"
#include "FkVertexCompo.h"
#include "FkCoordinateCompo.h"
#include "FkMatCompo.h"
#include "FkSizeCompo.h"
#include "FkPointFCompo.h"
#include "FkPointVertexCompo.h"
#include "FkReadPixelsProto.h"
#include "FkFuncCompo.h"

FK_IMPL_CLASS_TYPE(FkGraphicRenderAtom, FkSimpleAtom)

FkGraphicRenderAtom::FkGraphicRenderAtom() : FkSimpleAtom() {

}

FkGraphicRenderAtom::~FkGraphicRenderAtom() {

}

void FkGraphicRenderAtom::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicRenderAtom::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkReadPixelsProto, FkGraphicRenderAtom::_onReadPixels);
}

void FkGraphicRenderAtom::onConnect(std::shared_ptr<FkConnectChain> chain) {
    chain->next<FkEmptyQuark>();
}

FkResult FkGraphicRenderAtom::onCreate() {
    return FkSimpleAtom::onCreate();
}

FkResult FkGraphicRenderAtom::onDestroy() {
    return FkSimpleAtom::onDestroy();
}

FkResult FkGraphicRenderAtom::onStart() {
    return FkSimpleAtom::onStart();
}

FkResult FkGraphicRenderAtom::onStop() {
    return FkSimpleAtom::onStop();
}

FkResult FkGraphicRenderAtom::_onRenderRequest(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderRequestPrt, p);
    auto renderEngine = FkRenderContext::wrap(getContext())->getRenderEngine();
    FkAssert(renderEngine != nullptr, nullptr);
    auto canvas = proto->req->getCanvas();
    auto canvasSizeCompo = FK_FIND_COMPO(canvas, FkSizeCompo);
    if (canvasSizeCompo) {
        renderEngine->updateMaterial(canvas->material, canvasSizeCompo->size, FkColor::transparent());
    }
    for (auto &layer : proto->req->layers) {
        if (Fk_CANVAS_ID == layer->id) {
            continue;
        }
        _drawPoints(layer);
        auto materials = _makeRenderMaterials(layer);
        if (materials) {
            std::shared_ptr<FkDeviceEntity> device = std::make_shared<FkTexDeviceEntity>(canvas->material);
            auto ret = renderEngine->renderDevice(materials, device);
            FkAssert(FK_OK == ret, ret);
        } else {
            FkLogW(FK_DEF_TAG, "Skip render layer %d", layer->id);
        }
    }
    return _drawCanvas2Screen(canvas);
}

std::shared_ptr<FkMaterialEntity>
FkGraphicRenderAtom::_makeRenderMaterials(std::shared_ptr<FkGraphicLayer> &layer) {
    auto vtxCompo = FK_FIND_COMPO(layer, FkVertexCompo);
    auto coordCompo = FK_FIND_COMPO(layer, FkCoordinateCompo);
    auto matCompo = FK_FIND_COMPO(layer, FkMatCompo);
    if(vtxCompo && coordCompo && matCompo) {
        auto materials = std::make_shared<FkMaterialEntity>(layer->material);
        materials->addComponent(vtxCompo);
        materials->addComponent(coordCompo);
        materials->addComponent(matCompo);
        return materials;
    }
    return nullptr;
}

FkResult FkGraphicRenderAtom::_drawCanvas2Screen(std::shared_ptr<FkGraphicLayer> &canvas) {
    auto renderEngine = FkRenderContext::wrap(getContext())->getRenderEngine();
    FkAssert(renderEngine != nullptr, nullptr);
    auto materials = _makeRenderMaterials(canvas);
    if (materials) {
        std::shared_ptr<FkDeviceEntity> device = std::make_shared<FkScreenEntity>();
        auto ret = renderEngine->renderDevice(materials, device);
        FkAssert(FK_OK == ret, ret);
        return ret;
    } else {
        FkLogW(FK_DEF_TAG, "Skip render canvas %d");
    }
    return FK_FAIL;
}

FkResult FkGraphicRenderAtom::_drawPoints(std::shared_ptr<FkGraphicLayer> &layer) {
    std::vector<std::shared_ptr<FkComponent>> vec;
    if (FK_OK != layer->findComponents(vec, FkPointFCompo_Class::type)) {
        return FK_FAIL;
    }
    std::unordered_map<int64_t, std::vector<float>> map;
    for (auto &it : vec) {
        auto point = std::dynamic_pointer_cast<FkPointFCompo>(it);
        auto color = point->color.toInt();
        int64_t key = point->size;
        key = (key << 32);
        key |= (color & 0x00000000FFFFFFFF);
        map[key].emplace_back(point->value.x);
        map[key].emplace_back(point->value.y);
    }

    for (auto &itr : map) {
        auto key = itr.first;
        auto pointCompo = std::make_shared<FkPointFCompo>();
        pointCompo->size = key >> 32;
        pointCompo->color = FkColor::makeFrom((int32_t) (key & 0x00000000FFFFFFFF));
        auto vertex = std::make_shared<FkPointVertexCompo>();
        vertex->setup(itr.second.size() / 2, 2,
                      sizeof(float), itr.second.data());
        auto materials = std::make_shared<FkMaterialEntity>(layer->material);
        materials->addComponent(vertex);
        materials->addComponent(pointCompo);

        auto renderEngine = FkRenderContext::wrap(getContext())->getRenderEngine();
        std::shared_ptr<FkDeviceEntity> device = std::make_shared<FkTexDeviceEntity>(layer->material);
        return renderEngine->renderDevice(materials, device);
    }
    return FK_OK;
}

FkResult FkGraphicRenderAtom::_onReadPixels(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkReadPixelsProto, p);
    FkAssert(proto->layer != nullptr, FK_NPE);
    auto renderEngine = FkRenderContext::wrap(getContext())->getRenderEngine();
    FkAssert(renderEngine != nullptr, FK_NPE);
    auto materials = std::make_shared<FkMaterialEntity>(proto->layer->material);
    if (proto->size.isZero()) {
        auto sizeCompo = FK_FIND_COMPO(proto->layer, FkSizeCompo);
        proto->size = sizeCompo->size;
    }
    proto->buf = FkBuffer::alloc(proto->size.getWidth() * proto->size.getHeight() * 4);

    auto promise = std::make_shared<std::promise<int>>();
    auto bufDevice = std::make_shared<FkBufDeviceEntity>(proto->buf);
    bufDevice->setPosition(proto->pos.x, proto->pos.y);
    bufDevice->setSize(proto->size);
    std::shared_ptr<FkDeviceEntity> device = bufDevice;
    device->addComponent(std::make_shared<FkFuncCompo>([promise]() {
        promise->set_value(FK_OK);
    }));
    auto ret = renderEngine->renderDevice(materials, device);
    if (FK_OK != ret || FK_OK != promise->get_future().get()) {
        proto->buf = nullptr;
    }
    if (proto->finishCallback) {
        proto->finishCallback(proto->buf, proto->size);
        proto->finishCallback = nullptr;
    }
    return ret;
}
