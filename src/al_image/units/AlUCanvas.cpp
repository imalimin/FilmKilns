/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUCanvas.h"
#include "HwTexture.h"
#include "AlBitmapFactory.h"

#define TAG "AlUCanvas"

AlUCanvas::AlUCanvas(const string &alias) : Unit(alias) {
    registerEvent(EVENT_LAYER_RENDER_UPDATE_CANVAS,
                  reinterpret_cast<EventFunc>(&AlUCanvas::onUpdateCanvas));
    registerEvent(EVENT_LAYER_RENDER_CLEAR,
                  reinterpret_cast<EventFunc>(&AlUCanvas::onClear));
    registerEvent(EVENT_LAYER_RENDER_DRAW,
                  reinterpret_cast<EventFunc>(&AlUCanvas::onDraw));
    registerEvent(EVENT_LAYER_RENDER_SHOW,
                  reinterpret_cast<EventFunc>(&AlUCanvas::onShow));
    registerEvent(EVENT_CANVAS_SAVE, reinterpret_cast<EventFunc>(&AlUCanvas::onSave));
    registerEvent(EVENT_LAYER_RENDER_CROP_CANVAS,
                  reinterpret_cast<EventFunc>(&AlUCanvas::onCropCanvas));
}

AlUCanvas::~AlUCanvas() {
    this->onSaveListener = nullptr;
}

bool AlUCanvas::onCreate(AlMessage *msg) {
    texAllocator = new AlTexAllocator();
    return true;
}

bool AlUCanvas::onDestroy(AlMessage *msg) {
    mCanvas.release();
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    return true;
}

bool AlUCanvas::onUpdateCanvas(AlMessage *m) {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    AlSize *size = m->getObj<AlSize *>();
    _update(size->width, size->height, 0);
    return true;
}

bool AlUCanvas::onCropCanvas(AlMessage *m) {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    AlRectF *rectF = m->getObj<AlRectF *>();
    if (nullptr == rectF) {
        return false;
    }
    Logcat::i(TAG, "(%f,%f), (%f,%f)", rectF->left, rectF->top, rectF->right, rectF->bottom);
    _update(static_cast<int32_t>(mCanvas.getWidth() * rectF->getWidth()),
            static_cast<int32_t>(mCanvas.getHeight() * rectF->getHeight()), 0);
    return true;
}

bool AlUCanvas::onClear(AlMessage *m) {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    mCanvas.clear(1 == m->arg1);
    mDrawCount = 0;
    postEvent(AlMessage::obtain(EVENT_CANVAS_CLEAR_DONE));
    return false;
}

bool AlUCanvas::onDraw(AlMessage *m) {
    if (nullptr == m->obj) {
        return true;
    }
    Logcat::i(TAG, "%s(%d): %d", __FUNCTION__, __LINE__, m->arg1);
    AlImageLayerDrawModel *description = dynamic_cast<AlImageLayerDrawModel *>(m->obj);
    _newDefaultCanvas(description->getLayerSize());
    _draw(description);
    ++mDrawCount;
    _notifyDrawDone();
    return true;
}

bool AlUCanvas::onShow(AlMessage *m) {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    AlMessage *msg = AlMessage::obtain(EVENT_RENDER_FILTER);
    auto tex = mCanvas.getOutput();
    if (nullptr == tex) {
        Logcat::e(TAG, "%s(%d): Empty canvas", __FUNCTION__, __LINE__);
        return true;
    }
    msg->obj = tex;
    postEvent(msg);
    return true;
}

bool AlUCanvas::onSave(AlMessage *m) {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    std::string path = getString("output_path");
    if ("" == path || path.empty()) {
        if (onSaveListener) {
            onSaveListener(Hw::FAILED.code, "Failed", path.c_str());
        }
        return true;
    }
    auto output = mCanvas.getOutput();
    size_t size = static_cast<size_t>(output->getWidth() * output->getHeight() * 4);
    AlBuffer *buf = AlBuffer::alloc(size);
    if (Hw::SUCCESS != mCanvas.read(buf)) {
        Logcat::w(TAG, "%s(%d) read pixels failed", __FUNCTION__, __LINE__);
    }
    AlBitmapFactory::save(output->getWidth(), output->getHeight(), buf, path);
    delete buf;
    if (onSaveListener) {
        onSaveListener(Hw::SUCCESS.code, "Success", path.c_str());
    }
    return true;
}

void AlUCanvas::_newDefaultCanvas(AlSize size) {
    if (size.width <= 0 || size.height <= 0) {
        return;
    }
    if (mCanvas.getWidth() > 0 && mCanvas.getHeight() > 0) {
        return;
    }
    _update(size.width, size.height, 0);
}

void AlUCanvas::_draw(AlImageLayerDrawModel *description) {
    mCanvas.draw(description);
}

void AlUCanvas::setOnSaveListener(AlUCanvas::OnSaveListener listener) {
    this->onSaveListener = listener;
}

void AlUCanvas::_update(int32_t width, int32_t height, int32_t color) {
    AlSize *size = dynamic_cast<AlSize *>(getObject("canvas_size"));
    size->width = width;
    size->height = height;
    mCanvas.update(width, height, color, texAllocator);
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MEASURE_CANVAS_SIZE);
    msg->arg1 = mCanvas.getWidth();
    msg->arg2 = mCanvas.getHeight();
    postEvent(msg);
}

void AlUCanvas::_notifyDrawDone() {
    auto *msg = AlMessage::obtain(EVENT_CANVAS_DRAW_DONE);
    msg->arg1 = mDrawCount;
    postEvent(msg);
}
