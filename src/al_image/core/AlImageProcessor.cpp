/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageProcessor.h"
#include "AlULayer.h"
#include "AlULayerDescriptor.h"
#include "HwRender.h"
#include "HwScreen.h"
#include "ObjectBox.h"
#include "AlOperateFactory.h"
#include "AlCropOperateModel.h"
#include "AlContext.h"
#include "AlPositionTranslator.h"
#include "core/file/AlFileExporter.h"
#include "AlTarUtil.h"

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
    AlULayer *uLayer = new AlULayer(ALIAS_OF_IMAGE);
    AlUCanvas *uCanvas = new AlUCanvas(ALIAS_OF_LAYER_RENDER);
    registerAnUnit(uLayer);
    registerAnUnit(new AlULayerDescriptor(ALIAS_OF_DESCRIPTOR));
    registerAnUnit(uCanvas);
    registerAnUnit(new HwRender(ALIAS_OF_RENDER));
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    putObject("canvas_size", &mCanvasSize).to({ALIAS_OF_LAYER_RENDER});
    putObject("layers", ObjectBox::box(&mLayers)).to({ALIAS_OF_IMAGE});
    post([this] {
        this->context = new AlContext();
        this->putObject("AL_CONTEXT", this->context)
                .to({ALIAS_OF_IMAGE, ALIAS_OF_DESCRIPTOR, ALIAS_OF_LAYER_RENDER,
                     ALIAS_OF_RENDER, ALIAS_OF_SCREEN});
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
    putObject("layers", ObjectBox::box(&mLayers)).to({ALIAS_OF_IMAGE});
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
    if (mLayers.empty()) {
        return Hw::FAILED;
    }
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
    auto *layer = _getLayer(id);
    if (layer) {
        layer->setScale(scale.toFloat(), scale.toFloat());
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::postScale(int32_t id, AlRational ds) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer) {
        layer->setScale(layer->getScale().x * ds.toFloat(),
                        layer->getScale().y * ds.toFloat());
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::setRotation(int32_t id, AlRational r) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer) {
        layer->setRotation(r);
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::postRotation(int32_t id, AlRational dr) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer) {
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
    auto *layer = _getLayer(id);
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
    auto *layer = _getLayer(id);
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
    auto *layer = _getLayer(id);
    if (layer) {
        layer->setAlpha(alpha);
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

AlImageLayerModel *AlImageProcessor::_getLayer(int32_t id) {
    for (AlImageLayerModel *it : mLayers) {
        if (id == it->getId()) {
            return it;
        }
    }
    return nullptr;
}

void AlImageProcessor::transToCanvasPos(float &x, float &y) {
    AlPositionTranslator::translate(mWinSize, mCanvasSize, x, y);
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
    auto *layer = _getLayer(id);
    if (layer) {
        layer->removeCropOperator();
        transToCanvasPos(left, top);
        transToCanvasPos(right, bottom);
        layer->addOperator(AlOperateFactory::crop(left, top, right, bottom));
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::cropCanvas(float left, float top, float right, float bottom, int mode) {
//    postEvent(AlMessage::obtain(EVENT_LAYER_RENDER_CROP_CANVAS,
//                                new AlRectF(left, top, right, bottom)));
    transToCanvasPos(left, top);
    transToCanvasPos(right, bottom);
    AlRectF rectF(left, top, right, bottom);
    int dw = static_cast<int>(mCanvasSize.width * (rectF.getWidth() / 2.0f));
    int dh = static_cast<int>(mCanvasSize.height * (rectF.getHeight() / 2.0f));
    size_t size = mLayers.size();
    for (int i = 0; i < size; ++i) {
        auto *layer = mLayers[i];
        AlPointF pos(-(rectF.right + rectF.left) / 2.0f, -(rectF.top + rectF.bottom) / 2.0f);
        AlSize posPixels(static_cast<int>(mCanvasSize.width * pos.x),
                         static_cast<int>(mCanvasSize.height * pos.y));
        AlPointF nPos(posPixels.width / (float) dw, posPixels.height / (float) dh);
        layer->setPosition(layer->getPosition().x + nPos.x, layer->getPosition().y + nPos.y);
        Logcat::i(TAG, "(%f,%f), (%f,%f)",
                  -(rectF.right + rectF.left) / rectF.getWidth(),
                  -(rectF.top + rectF.bottom) / rectF.getHeight(),
                  nPos.x, nPos.y);
    }
    mCanvasSize.width = dw;
    mCanvasSize.height = dh;
    _notifyCanvasUpdate();
    invalidate();
    return Hw::SUCCESS;
}

HwResult AlImageProcessor::cancelCropLayer(int32_t id) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer && layer->removeCropOperator()) {
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::save(std::string path) {
    invalidate(3);
    putString("output_path", path).to({ALIAS_OF_LAYER_RENDER});
    postEvent(AlMessage::obtain(EVENT_LAYER_RENDER_SAVE));
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
    auto *layer = _getLayer(id);
    if (layer) {
        layer->removeAlignCropOperator();
        layer->addOperator(AlOperateFactory::alignCrop(r));
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::cancelAlignCrop(int32_t id) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer && layer->removeAlignCropOperator()) {
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
