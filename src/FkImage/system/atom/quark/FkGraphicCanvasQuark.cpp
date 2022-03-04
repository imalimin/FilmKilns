/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicCanvasQuark.h"
#include "FkRenderRequestPrt.h"
#include "FkSizeComponent.h"
#include "FkTexIDComponent.h"
#include "FkQuerySizeProto.h"
#include "FkTransComponent.h"
#include "FkScaleComponent.h"
#include "FkRotateComponent.h"
#include "FkScaleTypeComponent.h"
#include "FkMeasureTransProto.h"
#include "FkGraphicUpdateLayerPrt.h"
#include "FkDrawPointProto.h"
#include "FkPointComponent.h"
#include "FkVertexComponent.h"
#include "FkCoordinateComponent.h"
#include "FkLayerPostTransProto.h"
#include "FkLayerPostScaleProto.h"
#include "FkLayerPostRotateProto.h"

FkGraphicCanvasQuark::FkGraphicCanvasQuark() : FkGraphicLayerQuark() {
    FK_MARK_SUPER
}

FkGraphicCanvasQuark::~FkGraphicCanvasQuark() {

}

void FkGraphicCanvasQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    desc->clear();
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderRequestPrt, FkGraphicCanvasQuark::_onRenderRequest);
    FK_PORT_DESC_QUICK_ADD(desc, FkGraphicUpdateLayerPrt, FkGraphicCanvasQuark::_onUpdateLayer);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostTransProto, FkGraphicCanvasQuark::_onPostTranslate);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostScaleProto, FkGraphicCanvasQuark::_onPostScale);
    FK_PORT_DESC_QUICK_ADD(desc, FkLayerPostRotateProto, FkGraphicCanvasQuark::_onPostRotate);
    FK_PORT_DESC_QUICK_ADD(desc, FkMeasureTransProto, FkGraphicCanvasQuark::_onMeasureTrans);
    FK_PORT_DESC_QUICK_ADD(desc, FkDrawPointProto, FkGraphicCanvasQuark::_onDrawPoint);

    FK_PORT_DESC_QUICK_ADD(desc, FkQuerySizeProto, FkGraphicCanvasQuark::_onQueryCanvasSize);
}

FkResult FkGraphicCanvasQuark::onCreate() {
    auto canvas = newLayerEntity();
    layers.emplace(std::make_pair(Fk_CANVAS_ID, canvas));
    return FkGraphicLayerQuark::onCreate();
}

FkResult FkGraphicCanvasQuark::onDestroy() {
    return FkGraphicLayerQuark::onDestroy();
}

FkResult FkGraphicCanvasQuark::onStart() {
    return FkGraphicLayerQuark::onStart();
}

FkResult FkGraphicCanvasQuark::onStop() {
    return FkGraphicLayerQuark::onStop();
}

FkResult FkGraphicCanvasQuark::_onPostTranslate(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkLayerPostScaleProto, p);
    if (proto->layer != Fk_CANVAS_ID) {
        return FK_OK;
    }
    return FkGraphicLayerQuark::_onPostTranslate(p);
}

FkResult FkGraphicCanvasQuark::_onPostScale(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkLayerPostScaleProto, p);
    if (proto->layer != Fk_CANVAS_ID) {
        return FK_OK;
    }
    return FkGraphicLayerQuark::_onPostScale(p);
}

FkResult FkGraphicCanvasQuark::_onPostRotate(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkLayerPostScaleProto, p);
    if (proto->layer != Fk_CANVAS_ID) {
        return FK_OK;
    }
    return FkGraphicLayerQuark::_onPostRotate(p);
}

FkResult FkGraphicCanvasQuark::_onQueryCanvasSize(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkQuerySizeProto, p);
    auto compo = _getCanvas()->findComponent<FkSizeComponent>();
    if (compo) {
        proto->value = compo->size;
    }
    return FK_OK;
}

FkResult FkGraphicCanvasQuark::_onMeasureTrans(std::shared_ptr<FkProtocol> p) {
    auto proto = Fk_POINTER_CAST(FkMeasureTransProto, p);
    proto->canvas = std::make_shared<FkGraphicLayer>(*_getCanvas());
    return FK_OK;
}

std::shared_ptr<FkGraphicLayer> FkGraphicCanvasQuark::_getCanvas() {
    auto itr = layers.find(Fk_CANVAS_ID);
    FkAssert(layers.end() != itr, nullptr);
    return itr->second;
}
