/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlULayer.h"
#include "HwTexture.h"
#include "ObjectBox.h"
#include "AlLayerPair.h"
#include "AlRenderParams.h"
#include "AlTexManager.h"
#include "core/file/AlFileImporter.h"

#define TAG "AlULayer"

AlULayer::AlULayer(string alias) : Unit(alias) {
    registerEvent(EVENT_COMMON_INVALIDATE, reinterpret_cast<EventFunc>(&AlULayer::onInvalidate));
    registerEvent(EVENT_LAYER_ADD, reinterpret_cast<EventFunc>(&AlULayer::onAddLayer));
    registerEvent(EVENT_LAYER_REMOVE, reinterpret_cast<EventFunc>(&AlULayer::onRemoveLayer));
    registerEvent(EVENT_LAYER_MOVE, reinterpret_cast<EventFunc>(&AlULayer::onMoveLayer));
    registerEvent(EVENT_AIMAGE_IMPORT, reinterpret_cast<EventFunc>(&AlULayer::onImport));
    registerEvent(EVENT_AIMAGE_REDO, reinterpret_cast<EventFunc>(&AlULayer::onRedo));
    registerEvent(EVENT_AIMAGE_UNDO, reinterpret_cast<EventFunc>(&AlULayer::onUndo));
    registerEvent(EVENT_LAYER_MEASURE_CANVAS_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlULayer::_onCanvasUpdate));
    registerEvent(EVENT_SCREEN_UPDATE_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlULayer::_onWindowUpdate));
}

AlULayer::~AlULayer() {
    this->onAlxLoadListener = nullptr;
}

bool AlULayer::onCreate(AlMessage *msg) {
    AlLogI(TAG, "%d", 1);
    return true;
}

bool AlULayer::onDestroy(AlMessage *msg) {
    Logcat::e(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    mLayerManager.release();
    return true;
}

bool AlULayer::onAddLayer(AlMessage *msg) {
    int32_t id = mLayerManager.addLayer(msg->desc);
    postEvent(AlMessage::obtain(EVENT_LAYER_QUERY_NOTIFY, id));
    invalidate();
    return true;
}

bool AlULayer::onRemoveLayer(AlMessage *msg) {
    mLayerManager.removeLayer(msg->arg1);
    invalidate();
    return true;
}

bool AlULayer::onMoveLayer(AlMessage *msg) {
    return true;
}

bool AlULayer::onInvalidate(AlMessage *m) {
    _notifyAll(m->arg1);
    return true;
}

void AlULayer::_notifyAll(int32_t flags) {
    AlRenderParams params(flags);
    if (!mLayerManager.empty()) {
        int size = mLayerManager.size();
        for (int i = 0; i < size; ++i) {
            AlImageLayerModel *model = mLayerManager.getLayer(i);
            if (nullptr == model) continue;
            AlImageLayer *layer = mLayerManager.find(model->getId());
            if (nullptr == layer) continue;
            AlRenderParams p;
            p.setRenderScreen(false);
            ///只有最后一个图层绘制完之后才上屏
            if (i >= size - 1) {
                p = params;
            }
            if (0 == i) {
                p.setReqClear(true);
            }
            _notifyFilter(layer, model, p.toInt());
        }
    } else {
        ///没有图层时清空画布
        postEvent(AlMessage::obtain(EVENT_LAYER_RENDER_CLEAR));
        AlMessage *sMsg = AlMessage::obtain(EVENT_LAYER_RENDER_SHOW);
        sMsg->desc = "show";
        postEvent(sMsg);
    }
}

void AlULayer::_notifyFilter(AlImageLayer *layer, AlImageLayerModel *model, int32_t flags) {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_FILTER_RENDER, new AlLayerPair(layer, model));
    msg->arg1 = flags;
    msg->desc = "filter";
    postEvent(msg);
}

bool AlULayer::onImport(AlMessage *m) {
//    std::string path = m->desc;
//    AlImageCanvasModel canvas;
//    std::vector<AlImageLayerModel *> layers;
//    AlFileImporter importer;
//    if (Hw::SUCCESS != importer.importFromFile(path, &canvas, &layers)
//        || layers.empty() || canvas.getWidth() <= 0 || canvas.getHeight() <= 0) {
//        return true;
//    }
//    mLayerManager.replaceAll(&layers);
//    layers.clear();
//    AlMessage *msg = AlMessage::obtain(EVENT_CANVAS_RESIZE, nullptr,
//                                       AlMessage::QUEUE_MODE_FIRST_ALWAYS);
//    msg->obj = new AlSize(canvas.getWidth(), canvas.getHeight());
//    postEvent(msg);
//    _notifyAll();
//    if (onAlxLoadListener) {
//        onAlxLoadListener(mLayerManager.getMaxId());
//    }
    return true;
}

bool AlULayer::onRedo(AlMessage *m) {
    return true;
}

bool AlULayer::onUndo(AlMessage *m) {
    return true;
}

bool AlULayer::_onWindowUpdate(AlMessage *msg) {
    int32_t width = msg->arg1;
    int32_t height = static_cast<int>(msg->arg2);
    mWinCoord.setWide(width, height);
    _updateCoordination();
    AlLogI(TAG, "%dx%d", width, height);
    return true;

}

bool AlULayer::_onCanvasUpdate(AlMessage *msg) {
    int32_t width = msg->arg1;
    int32_t height = static_cast<int>(msg->arg2);
    mCanvasCoord.setWide(width, height);
    _updateCoordination();
    AlLogI(TAG, "%dx%d", width, height);
    return true;
}

void AlULayer::_saveStep() {

}

AlVec2 AlULayer::transWin2Layer(AlImageLayerModel *model, float x, float y) {
    auto *layer = mLayerManager.find(model->getId());
    if (nullptr == layer) {
        return AlVec2(x, y);
    }
    AlVec2 vec(x, y);
    mWinCoord.translate(&vec, &mCanvasCoord);
    float tx = vec.x, ty = vec.y;
    Al2DCoordinate layerCoord(layer->getWidth(), layer->getHeight());
    layerCoord.setScale(model->getScale().x, model->getScale().y);
    layerCoord.setRotation(model->getRotation());
    layerCoord.setPosition(model->getPosition().x, model->getPosition().y);
    mCanvasCoord.translate(&vec, &layerCoord);
    AlLogI(TAG, "(%f, %f) -> (%f, %f) -> (%f, %f)", x, y, tx, ty, vec.x, vec.y);
    vec.y = vec.y;
    return vec;
}

AlVec2 AlULayer::transWin2Canvas(float x, float y) {
    AlVec2 vec(x, y);
    mWinCoord.translate(&vec, &mCanvasCoord);
    return vec;
}

void AlULayer::_updateCoordination() {
    AlSize ws = mWinCoord.getRegion();
    AlSize cs = mCanvasCoord.getRegion();
    if (ws.width > 0 && ws.height > 0
        && cs.width > 0 && cs.height > 0) {
        float wRatio = ws.ratio();
        float cRatio = cs.ratio();
        float scale = 1.f;
        /// 由于Canvas在Win中默认会以centerInside的状态显示
        /// 所以这里需要根据两个的比例来计算实际的缩放系数
        if (wRatio > cRatio) {
            scale = ws.height / (float) cs.height;
        } else {
            scale = ws.width / (float) cs.width;
        }
        mCanvasCoord.setScale(scale, scale);
    }
}

void AlULayer::setOnAlxLoadListener(AlULayer::OnAlxLoadListener listener) {
    this->onAlxLoadListener = listener;
}

AlImageLayerModel *AlULayer::findLayerModel(float x, float y) {
    AlVec2 vec(x, y);
    mWinCoord.translate(&vec, &mCanvasCoord);
    return mLayerManager.findModel(vec.x, vec.y);
}

AlImageLayerModel *AlULayer::findLayerModel(int32_t layerId) {
    return mLayerManager.findModel(layerId);
}

void AlULayer::invalidate() {
    postEvent(AlMessage::obtain(EVENT_COMMON_INVALIDATE, AlMessage::QUEUE_MODE_UNIQUE));
}

AlSize AlULayer::getCanvasSize() {
    return mCanvasCoord.getRegion();
}

void AlULayer::cropCanvasAndStayLoc(AlSize *src, AlSize *dest, AlPointF *anchor) {
    auto size = mLayerManager.size();
    for (int i = 0; i < size; ++i) {
        auto *model = mLayerManager.findModelByIndex(i);
        AlSize posPixels(static_cast<int>(src->width * (model->getPosition().x + anchor->x)),
                         static_cast<int>(src->height * (model->getPosition().y + anchor->y)));
        AlPointF nPos(posPixels.width / (float) dest->width,
                      posPixels.height / (float) dest->height);
        model->setPosition(nPos.x, nPos.y);
    }
}
