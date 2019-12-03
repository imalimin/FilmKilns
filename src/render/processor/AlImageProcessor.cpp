/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImageProcessor.h"
#include "AlImage.h"
#include "HwRender.h"
#include "HwScreen.h"
#include "ObjectBox.h"

#define TAG "AlImageProcessor"

AlImageProcessor::AlImageProcessor() : HwAbsProcessor("AlImageProcessor") {
    registerAnUnit(new AlImage(ALIAS_OF_IMAGE));
    registerAnUnit(new HwRender(ALIAS_OF_RENDER));
    registerAnUnit(new HwScreen(ALIAS_OF_SCREEN));
    putObject("canvas", &mCanvasModel).to({ALIAS_OF_IMAGE});
    putObject("layers", ObjectBox::box(&mLayers)).to({ALIAS_OF_IMAGE});
    Message *msg = new Message(EVENT_COMMON_PREPARE);
    postEvent(msg);
}

AlImageProcessor::~AlImageProcessor() {

}

void AlImageProcessor::onDestroy() {
    HwAbsProcessor::onDestroy();
}

void AlImageProcessor::prepare(HwWindow *win) {
    Message *msg = new Message(EVENT_COMMON_PREPARE);
    msg->obj = new ObjectBox(new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlImageProcessor::updateWindow(HwWindow *win) {
    mWinSize.width = win->getWidth();
    mWinSize.height = win->getHeight();
    Message *msg = new Message(EVENT_SCREEN_UPDATE_WINDOW);
    msg->obj = ObjectBox::box(new NativeWindow(win, nullptr));
    postEvent(msg);
}

void AlImageProcessor::setCanvas(int32_t w, int32_t h, int32_t color) {
    mCanvasModel.set(w, h, color);
    _notifyCanvasUpdate();
}

void AlImageProcessor::invalidate() {
    postEvent(new Message(EVENT_COMMON_INVALIDATE, nullptr, Message::QUEUE_MODE_UNIQUE));
}

void AlImageProcessor::_notifyCanvasUpdate() {
    putObject("canvas", &mCanvasModel).to({ALIAS_OF_IMAGE});
    postEvent(new Message(EVENT_AIMAGE_UPDATE_CANVAS));
}

void AlImageProcessor::_notifyLayerUpdate() {
    putObject("layers", ObjectBox::box(&mLayers)).to({ALIAS_OF_IMAGE});
    postEvent(new Message(EVENT_AIMAGE_UPDATE_LAYER));
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

HwResult AlImageProcessor::setRotation(int32_t id, float rotation) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer) {
        layer->setRotation(rotation);
        invalidate();
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult AlImageProcessor::setTranslate(int32_t id, float x, float y) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    auto *layer = _getLayer(id);
    if (layer) {
        calculatePosition(x, y);
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
        calculatePosition(dx, dy);
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

void AlImageProcessor::calculatePosition(float &x, float &y) {
    float winRatio = mWinSize.ratio();
    float cRatio = mCanvasModel.getWidth() / (float) mCanvasModel.getHeight();
    if (winRatio > cRatio) {
        float scale = cRatio / winRatio;
        x = x / scale;
    } else {
        float scale = winRatio / cRatio;
        y = y / scale;
    }
}

/// 根据window坐标获取对应的Layer，还需要适配缩放旋转的情况
/// \param x x坐标，x=[-1,1]
/// \param y y坐标，y=[-1,1]
/// \return 对应的图层id，否则-1
int32_t AlImageProcessor::getLayer(float x, float y) {
    std::lock_guard<std::mutex> guard(mLayerMtx);
    calculatePosition(x, y);
    size_t size = mLayers.size();
    for (int i = size - 1; i >= 0; --i) {
        auto *it = mLayers.at(static_cast<unsigned int>(i));
//        Logcat::i(TAG, "%s(%d): %f, %f", __FUNCTION__, __LINE__, x, y);
        if (it && it->getQuad().inside(AlPointF(x, y))) {
//            it->dump();
            return it->getId();
        }
    }
    return Hw::FAILED.code;
}

HwResult AlImageProcessor::save(std::string path) {
    putString("output_path", path).to({ALIAS_OF_IMAGE});
    Message *msg = new Message(EVENT_AIMAGE_SAVE);
    postEvent(msg);
    return Hw::SUCCESS;
}
