/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlLayerRender.h"
#include "HwTexture.h"
#include "AlBitmapFactory.h"

#define TAG "AlLayerRender"

AlLayerRender::AlLayerRender(const string &alias) : Unit(alias) {
    registerEvent(EVENT_LAYER_RENDER_UPDATE_CANVAS,
                  reinterpret_cast<EventFunc>(&AlLayerRender::onUpdateCanvas));
    registerEvent(EVENT_LAYER_RENDER_CLEAR,
                  reinterpret_cast<EventFunc>(&AlLayerRender::onClear));
    registerEvent(EVENT_LAYER_RENDER_DRAW,
                  reinterpret_cast<EventFunc>(&AlLayerRender::onDraw));
    registerEvent(EVENT_LAYER_RENDER_SHOW,
                  reinterpret_cast<EventFunc>(&AlLayerRender::onShow));
    registerEvent(EVENT_LAYER_RENDER_SAVE, reinterpret_cast<EventFunc>(&AlLayerRender::onSave));
}

AlLayerRender::~AlLayerRender() {
    this->onSaveListener = nullptr;
}

bool AlLayerRender::onCreate(AlMessage *msg) {
    texAllocator = new AlTexAllocator();
    return true;
}

bool AlLayerRender::onDestroy(AlMessage *msg) {
    mCanvas.release();
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    return true;
}

bool AlLayerRender::onUpdateCanvas(AlMessage *m) {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    AlSize *size = m->getObj<AlSize *>();
    _update(size->width, size->height, 0);
    return true;
}

bool AlLayerRender::onClear(AlMessage *m) {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    mCanvas.clear(1 == m->arg1);
    return false;
}

bool AlLayerRender::onDraw(AlMessage *m) {
    if (nullptr == m->obj) {
        return true;
    }
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    AlImageLayerDrawModel *description = dynamic_cast<AlImageLayerDrawModel *>(m->obj);
    _newDefaultCanvas(description->getLayerSize());
    _draw(description);
//    onShow(nullptr);
    return true;
}

bool AlLayerRender::onShow(AlMessage *m) {
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

bool AlLayerRender::onSave(AlMessage *m) {
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
    mCanvas.read(buf);
    AlBitmapFactory::save(output->getWidth(), output->getHeight(), buf, path);
    delete buf;
    if (onSaveListener) {
        onSaveListener(Hw::SUCCESS.code, "Success", path.c_str());
    }
    return true;
}

void AlLayerRender::_newDefaultCanvas(AlSize size) {
    if (size.width <= 0 || size.height <= 0) {
        return;
    }
    if (mCanvas.getWidth() > 0 && mCanvas.getHeight() > 0) {
        return;
    }
    _update(size.width, size.height, 0);
}

void AlLayerRender::_draw(AlImageLayerDrawModel *description) {
    mCanvas.draw(description);
}

void AlLayerRender::setOnSaveListener(AlLayerRender::OnSaveListener listener) {
    this->onSaveListener = listener;
}

void AlLayerRender::_update(int32_t width, int32_t height, int32_t color) {
    AlSize *size = dynamic_cast<AlSize *>(getObject("canvas_size"));
    size->width = width;
    size->height = height;
    mCanvas.update(width, height, color, texAllocator);
    AlMessage *msg = AlMessage::obtain(EVENT_LAYER_MEASURE_CANVAS_SIZE);
    msg->arg1 = mCanvas.getWidth();
    msg->arg2 = mCanvas.getHeight();
    postEvent(msg);
}
