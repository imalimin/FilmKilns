/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageProcessor.h"
#include "AlULayerWithOpt.h"
#include "AlULayerFilter.h"
#include "AlULayerDescriptor.h"
#include "HwScreen.h"
#include "ObjectBox.h"
#include "AlLayerActionFactory.h"
#include "AlMCropAction.h"
#include "AlContext.h"
#include "AlCoordsTranslator.h"
#include "core/file/AlFileExporter.h"
#include "AlTarUtil.h"
#include "AlMath.h"
#include "AlRenderParams.h"
#include "AlPaintDesc.h"
#include "AlOperateScale.h"
#include "AlOperateRotate.h"
#include "AlOperateTrans.h"

#define TAG "AlImageProcessor"

AlImageProcessor::AlImageProcessor() : AlAbsProcessor("AlImageProcessor") {
//    tar_t *archive = nullptr;
//    int count = tar_read_file("/sdcard/test.tar", &archive);
//    if (archive) {
//        for (int i = 0; i < count; ++i) {
//            Logcat::i(TAG, "%s", archive[i].name);
//        }
//        tar_free(archive);
//    }
    putObject("layers", ObjectBox::box(&mLayers)).to({ALIAS_OF_LAYER});
    AlULayer *uLayer = new AlULayerWithOpt(ALIAS_OF_LAYER);
    AlUCanvas *uCanvas = new AlUCanvas(ALIAS_OF_CANVAS);
    registerAnUnit(uLayer);
    registerAnUnit(new AlULayerFilter(ALIAS_OF_FILTER));
    registerAnUnit(new AlULayerDescriptor(ALIAS_OF_DESCRIPTOR));
    registerAnUnit(uCanvas);
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    uCanvas->setOnSaveListener([this](int32_t code, const char *msg, const char *path) {
        if (this->onSaveListener) {
            this->onSaveListener(code, msg, path);
        }
    });
    uLayer->setOnAlxLoadListener([this](int32_t id) {
        mLayerIdCreator.reset(id);
    });
    registerEvent(EVENT_LAYER_MEASURE_CANVAS_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlImageProcessor::_onCanvasUpdate));
    registerEvent(EVENT_LAYER_QUERY_NOTIFY,
                  reinterpret_cast<EventFunc>(&AlImageProcessor::_onLayerQuery));
}

AlImageProcessor::~AlImageProcessor() {
    size_t size = mLayers.size();
    for (int i = 0; i < size; ++i) {
        AlImageLayerModel *it = mLayers[i];
        delete it;
    }
    mLayers.clear();
    this->onSaveListener = nullptr;
}

void AlImageProcessor::onCreate() {
    AlAbsProcessor::onCreate();
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    this->aBaseCtx = AlEgl::offScreen();
    this->context = new AlContext();
    this->putObject("AL_CONTEXT", this->context)
            .to({ALIAS_OF_LAYER, ALIAS_OF_DESCRIPTOR, ALIAS_OF_CANVAS, ALIAS_OF_SCREEN});
}

void AlImageProcessor::onDestroy() {
    AlAbsProcessor::onDestroy();
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    delete this->context;
    this->context = nullptr;
    delete this->aBaseCtx;
    this->aBaseCtx = nullptr;
}

void AlImageProcessor::updateWindow(HwWindow *win) {
    mWinSize.width = win->getWidth();
    mWinSize.height = win->getHeight();
    AlMessage *msg = AlMessage::obtain(EVENT_SCREEN_UPDATE_WINDOW, new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlImageProcessor::setCanvas(int32_t w, int32_t h, int32_t color) {
//    float scale = w / (float) mCanvasSize.width;
//    AlSize dest(w, static_cast<int>(mCanvasSize.height * scale));
//    size_t size = mLayers.size();
//    for (int i = 0; i < size; ++i) {
//        auto *layer = mLayers[i];
//        layer->setScale(layer->getScale().x * scale, layer->getScale().y * scale);
//    }
    mCanvasSize.width = w;
    mCanvasSize.height = h;
    _notifyCanvasUpdate();
    invalidate();
}

void AlImageProcessor::invalidate(int32_t flag) {
    AlMessage *msg = AlMessage::obtain(EVENT_COMMON_INVALIDATE, nullptr,
                                       AlMessage::QUEUE_MODE_UNIQUE);
    msg->arg1 = flag;
    postEvent(msg);
}

void AlImageProcessor::_notifyCanvasUpdate() {
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_RENDER_UPDATE_CANVAS);
    msg->obj = new AlSize(mCanvasSize.width, mCanvasSize.height);
    postEvent(msg);
}

void AlImageProcessor::_notifyLayerUpdate() {
    putObject("layers", ObjectBox::box(&mLayers)).to({ALIAS_OF_LAYER});
    postEvent(AlMessage::obtain(EVENT_AIMAGE_UPDATE_LAYER));
}

int32_t AlImageProcessor::addLayer(const char *path) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    std::string str(path);
    auto *layer = AlImageLayerModel::create(&mLayerIdCreator, str);
    if (nullptr == layer) {
        return Hw::FAILED.code;
    }
    mLayers.push_back(layer);
    _notifyLayerUpdate();
    invalidate();
    return layer->getId();
}

HwResult AlImageProcessor::removeLayer(int32_t id) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    size_t size = mLayers.size();
    for (int i = 0; i < size; ++i) {
        AlImageLayerModel *it = mLayers[i];
        if (id == it->getId()) {
            auto itr = mLayers.begin();
            std::advance(itr, i);
            mLayers.erase(itr);
            delete it;
            _notifyLayerUpdate();
            invalidate();
            return Hw::SUCCESS;
        }
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::moveLayerIndex(int32_t id, int32_t index) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    if (mLayers.empty()) {
        return Hw::FAILED;
    }
    size_t size = mLayers.size();
    index = std::max(index, 0);
    index = std::min<int32_t>(index, size - 1);
    for (int i = 0; i < size; ++i) {
        AlImageLayerModel *it = mLayers[i];
        if (id == it->getId()) {
            auto itr = mLayers.begin();
            std::advance(itr, i);
            mLayers.erase(itr);
            mLayers.insert(mLayers.begin() + index, it);
            invalidate();
            return Hw::SUCCESS;
        }
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::setScale(int32_t id, AlRational scale) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_SCALE,
                                  new AlOperateScale(id, scale, AlPointF(0, 0)),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::postScale(int32_t id, AlRational ds, AlPointF anchor) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_SCALE_POST,
                                  new AlOperateScale(id, ds, anchor),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::setRotation(int32_t id, AlRational r) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_ROTATE,
                                  new AlOperateRotate(id, r, AlPointF(0, 0)),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::postRotation(int32_t id, AlRational dr, AlPointF anchor) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_ROTATE_POST,
                                  new AlOperateRotate(id, dr, anchor),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::setTranslate(int32_t id, float x, float y) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_TRANS,
                                  new AlOperateTrans(id, x, y),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::postTranslate(int32_t id, float dx, float dy) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_TRANS_POST,
                                  new AlOperateTrans(id, dx, dy),
                                  AlMessage::QUEUE_MODE_UNIQUE);
    postEvent(msg);
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::setAlpha(int32_t id, float alpha) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer) {
        layer->setAlpha(alpha);
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

AlImageLayerModel *AlImageProcessor::_findLayer(int32_t id) {
    for (AlImageLayerModel *it : mLayers) {
        if (id == it->getId()) {
            return it;
        }
    }
    return nullptr;
}

int32_t AlImageProcessor::getLayer(float x, float y) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_QUERY,
                                  new AlOperateTrans(0, x, y));
    postEvent(msg);
    mQueryLock.wait(500000);
    return mCurLayerId;
}

HwResult AlImageProcessor::cropLayer(int32_t id, float left, float top, float right, float bottom) {
//    std::lock_guard<std::mutex> guard(mLayerMtx);
//    auto *layer = _findLayer(id);
//    if (layer) {
//        layer->removeCropAction();
//        transToCanvasPos(left, top);
//        transToCanvasPos(right, bottom);
//        layer->addAction(AlLayerActionFactory::crop(left, top, right, bottom));
//        invalidate();
//        return Hw::SUCCESS;
//    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::cropCanvas(float left, float top, float right, float bottom) {
//    postEvent(AlMessage::obtain(EVENT_LAYER_RENDER_CROP_CANVAS,
//                                new AlRectF(left, top, right, bottom)));
//    transToCanvasPos(left, top);
//    transToCanvasPos(right, bottom);
//    AlRectF rectF(left, top, right, bottom);
//    AlSize dest(static_cast<int>(mCanvasSize.width * (rectF.getWidth() / 2.0f)),
//                static_cast<int>(mCanvasSize.height * (rectF.getHeight() / 2.0f)));
//    AlPointF anchor(-(rectF.right + rectF.left) / 2.0f, -(rectF.top + rectF.bottom) / 2.0f);
//    size_t size = mLayers.size();
//    for (int i = 0; i < size; ++i) {
//        auto *layer = mLayers[i];
//        AlCoordsTranslator::changeCanvasStayLoc(&mCanvasSize, &dest, &anchor, layer);
//    }
//    mCanvasSize.width = dest.width;
//    mCanvasSize.height = dest.height;
//    _notifyCanvasUpdate();
//    invalidate();
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::cancelCropLayer(int32_t id) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer && layer->removeCropAction()) {
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::save(std::string path) {
    putString("output_path", path).to({ALIAS_OF_CANVAS});
    AlRenderParams params;
    params.setRenderScreen(false);
    params.setTransparent(true);
    params.setReqSave(true);
    invalidate(params.toInt());
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::exportFile(std::string path) {
    AlFileExporter exporter;
    AlImageCanvasModel canvas;
    canvas.set(mCanvasSize.width, mCanvasSize.height, 0);
    return exporter.exportAsFile(&canvas, &mLayers, path);
}

HwResult AlImageProcessor::importFile(std::string path) {
    AlMessage *msg = AlMessage::obtain(EVENT_AIMAGE_IMPORT);
    msg->desc = path;
    postEvent(msg);
    invalidate();
    return Hw::SUCCESS;
}

void AlImageProcessor::setOnSaveListener(AlUCanvas::OnSaveListener listener) {
    this->onSaveListener = listener;
}

HwResult AlImageProcessor::ensureAlignCrop(int32_t id, AlRational r) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer) {
        layer->removeAlignCropAction();
        layer->addAction(AlLayerActionFactory::alignCrop(r));
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::cancelAlignCrop(int32_t id) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer && layer->removeAlignCropAction()) {
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::redo() {
    postEvent(AlMessage::obtain(EVENT_AIMAGE_REDO));
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::undo() {
    postEvent(AlMessage::obtain(EVENT_AIMAGE_UNDO));
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::paint(int32_t id, float x, float y, bool painting) {
    auto *msg = AlMessage::obtain(EVENT_LAYER_PAINT);
    msg->obj = new AlPaintDesc(id, x, y, painting);
    postEvent(msg);
    return Hw::SUCCESS;
}

bool AlImageProcessor::_onCanvasUpdate(AlMessage *msg) {
    mCanvasSize.width = msg->arg1;
    mCanvasSize.height = static_cast<int>(msg->arg2);
    return true;
}

bool AlImageProcessor::_onLayerQuery(AlMessage *msg) {
    AlLogI(TAG, "%d", msg->arg1);
    mCurLayerId = msg->arg1;
    mQueryLock.notify();
    return true;
}
