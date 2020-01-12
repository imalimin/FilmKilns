/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageProcessor.h"
#include "AlULayer.h"
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
#include "AlMPaintAction.h"

#define TAG "AlImageProcessor"

AlImageProcessor::AlImageProcessor() : HwAbsProcessor("AlImageProcessor") {
//    tar_t *archive = nullptr;
//    int count = tar_read_file("/sdcard/test.tar", &archive);
//    if (archive) {
//        for (int i = 0; i < count; ++i) {
//            Logcat::i(TAG, "%s", archive[i].name);
//        }
//        tar_free(archive);
//    }
    AlULayer *uLayer = new AlULayer(ALIAS_OF_LAYER);
    AlUCanvas *uCanvas = new AlUCanvas(ALIAS_OF_CANVAS);
    registerAnUnit(uLayer);
    registerAnUnit(new AlULayerFilter(ALIAS_OF_FILTER));
    registerAnUnit(new AlULayerDescriptor(ALIAS_OF_DESCRIPTOR));
    registerAnUnit(uCanvas);
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    putObject("canvas_size", &mCanvasSize).to({ALIAS_OF_CANVAS});
    putObject("layers", ObjectBox::box(&mLayers)).to({ALIAS_OF_LAYER});
    post([this] {
        this->aBasCtx = AlEgl::offScreen();
        this->context = new AlContext();
        this->putObject("AL_CONTEXT", this->context)
                .to({ALIAS_OF_LAYER, ALIAS_OF_DESCRIPTOR, ALIAS_OF_CANVAS, ALIAS_OF_SCREEN});
    });
    prepare();
    uCanvas->setOnSaveListener([this](int32_t code, const char *msg, const char *path) {
        if (this->onSaveListener) {
            this->onSaveListener(code, msg, path);
        }
    });
    uLayer->setOnAlxLoadListener([this](int32_t id) {
        mLayerIdCreator.reset(id);
    });
}

AlImageProcessor::~AlImageProcessor() {
    post([this] {
        delete this->context;
        this->context = nullptr;
        delete this->aBasCtx;
        this->aBasCtx = nullptr;
    });
    size_t size = mLayers.size();
    for (int i = 0; i < size; ++i) {
        AlImageLayerModel *it = mLayers[i];
        delete it;
    }
    mLayers.clear();
    this->onSaveListener = nullptr;
}

void AlImageProcessor::onDestroy() {
    HwAbsProcessor::onDestroy();
}

void AlImageProcessor::updateWindow(HwWindow *win) {
    mWinSize.width = win->getWidth();
    mWinSize.height = win->getHeight();
    AlMessage *msg = AlMessage::obtain(EVENT_SCREEN_UPDATE_WINDOW, new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlImageProcessor::setCanvas(int32_t w, int32_t h, int32_t color) {
    float scale = w / (float) mCanvasSize.width;
    AlSize dest(w, static_cast<int>(mCanvasSize.height * scale));
    size_t size = mLayers.size();
    for (int i = 0; i < size; ++i) {
        auto *layer = mLayers[i];
        layer->setScale(layer->getScale().x * scale, layer->getScale().y * scale);
    }
    mCanvasSize.width = dest.width;
    mCanvasSize.height = dest.height;
    Logcat::i(TAG, "%s(%d) %dx%d", __FUNCTION__, __LINE__, dest.width, dest.height);
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
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer) {
        layer->setScale(scale.toFloat(), scale.toFloat());
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::postScale(int32_t id, AlRational ds, AlPointF anchor) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer) {
        transToCanvasPos(anchor.x, anchor.y);
        float scale = ds.toFloat();
        float dx = anchor.x - layer->getPosition().x;
        float dy = anchor.y - layer->getPosition().y;
        float x = dx * (1.0f - scale);
        float y = dy * (1.0f - scale);
        layer->setPosition(layer->getPosition().x + x, layer->getPosition().y + y);
        layer->setScale(layer->getScale().x * scale,
                        layer->getScale().y * scale);
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::setRotation(int32_t id, AlRational r) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer) {
        layer->setRotation(r);
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::postRotation(int32_t id, AlRational dr, AlPointF anchor) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer) {
        transToCanvasPos(anchor.x, anchor.y);
        Logcat::i(TAG, "anchor %f,%f", anchor.x, anchor.y);

//        float alpha = dr.toFloat() * AlMath::PI;
//        float dx = anchor.x - layer->getPosition().x;
//        float dy = anchor.y - layer->getPosition().y;
//
//        float x = dx * cosf(alpha) - dy * sinf(alpha);
//        float y = dx * sinf(alpha) + dy * cosf(alpha);
//        layer->setPosition(anchor.x - x,
//                           anchor.y - y);

        ///TODO 还可以提高精度
        auto nr = layer->getRotation().toFloat() + dr.toFloat();
        auto r = AlRational(static_cast<int32_t>(nr * 100000), 100000);
        layer->setRotation(r);
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::setTranslate(int32_t id, float x, float y) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer) {
        transToCanvasPos(x, y);
        layer->setPosition(x, y);
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::postTranslate(int32_t id, float dx, float dy) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer) {
        transToCanvasPos(dx, dy);
        layer->setPosition(layer->getPosition().x + dx, layer->getPosition().y + dy);
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
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

void AlImageProcessor::transToCanvasPos(float &x, float &y) {
    AlCoordsTranslator::translate(mWinSize, mCanvasSize, x, y);
}

int32_t AlImageProcessor::getLayer(float x, float y) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    transToCanvasPos(x, y);
    size_t size = mLayers.size();
    for (int i = size - 1; i >= 0; --i) {
        auto *layer = mLayers[i];
        if (layer && layer->getQuad().inside(AlPointF(x, y))) {
            return layer->getId();
        }
    }
    return Hw::FAILED.code;
}

HwResult AlImageProcessor::cropLayer(int32_t id, float left, float top, float right, float bottom) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer) {
        layer->removeCropAction();
        transToCanvasPos(left, top);
        transToCanvasPos(right, bottom);
        layer->addAction(AlLayerActionFactory::crop(left, top, right, bottom));
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::cropCanvas(float left, float top, float right, float bottom) {
//    postEvent(AlMessage::obtain(EVENT_LAYER_RENDER_CROP_CANVAS,
//                                new AlRectF(left, top, right, bottom)));
    transToCanvasPos(left, top);
    transToCanvasPos(right, bottom);
    AlRectF rectF(left, top, right, bottom);
    AlSize dest(static_cast<int>(mCanvasSize.width * (rectF.getWidth() / 2.0f)),
                static_cast<int>(mCanvasSize.height * (rectF.getHeight() / 2.0f)));
    AlPointF anchor(-(rectF.right + rectF.left) / 2.0f, -(rectF.top + rectF.bottom) / 2.0f);
    size_t size = mLayers.size();
    for (int i = 0; i < size; ++i) {
        auto *layer = mLayers[i];
        AlCoordsTranslator::changeCanvasStayLoc(&mCanvasSize, &dest, &anchor, layer);
    }
    mCanvasSize.width = dest.width;
    mCanvasSize.height = dest.height;
    _notifyCanvasUpdate();
    invalidate();
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
    invalidate(7);
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

HwResult AlImageProcessor::paint(int32_t id, AlPointF pointF, bool painting) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _findLayer(id);
    if (layer) {
        transToCanvasPos(pointF.x, pointF.y);
        pointF.y = -pointF.y;
        AlAbsMAction *action = nullptr;
        auto *actions = layer->getAllActions();
        size_t size = actions->size();
        for (int i = 0; i < size; ++i) {
            AlAbsMAction *a = (*actions)[i];
            if (typeid(AlMPaintAction) == typeid(*a)) {
                action = a;
                break;
            }
        }
        if (nullptr == action) {
            action = AlLayerActionFactory::paint(0.01f, AlColor(0xff0000));
            layer->addAction(action);
        }
        dynamic_cast<AlMPaintAction *>(action)->paint(pointF);
        if (!painting) {
            dynamic_cast<AlMPaintAction *>(action)->newPath();
        }
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}
