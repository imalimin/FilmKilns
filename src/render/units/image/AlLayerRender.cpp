/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlLayerRender.h"
#include "HwTexture.h"
#include "HwBitmapFactory.h"

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

bool AlLayerRender::onCreate(Message *msg) {
    texAllocator = new TextureAllocator();
    return true;
}

bool AlLayerRender::onDestroy(Message *msg) {
    mCanvas.release();
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    return true;
}

bool AlLayerRender::onUpdateCanvas(Message *m) {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    auto model = _getCanvas();
    mCanvas.update(model->getWidth(), model->getHeight(), model->getColor(),
                   texAllocator);
    Message *msg = new Message(EVENT_LAYER_MEASURE_CANVAS_SIZE);
    msg->arg1 = model->getWidth();
    msg->arg2 = model->getHeight();
    postEvent(msg);
    return true;
}

bool AlLayerRender::onClear(Message *m) {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    mCanvas.clear(1 == m->arg1);
    return false;
}

bool AlLayerRender::onDraw(Message *m) {
    if (nullptr == m->obj) {
        return true;
    }
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    AlImageLayerDrawModel *desc = dynamic_cast<AlImageLayerDrawModel *>(m->obj);
    _newDefaultCanvas(desc->getLayerSize());
    _draw(desc);
//    onShow(nullptr);
    return true;
}

bool AlLayerRender::onShow(Message *m) {
    Logcat::i(TAG, "%s(%d)", __FUNCTION__, __LINE__);
    Message *msg = new Message(EVENT_RENDER_FILTER, nullptr);
    auto tex = mCanvas.getOutput();
    if (nullptr == tex) {
        Logcat::e(TAG, "%s(%d): Empty canvas", __FUNCTION__, __LINE__);
        return true;
    }
    msg->obj = HwTexture::wrap(tex->target(), tex->texId(),
                               tex->getWidth(),
                               tex->getHeight(),
                               tex->fmt());
    postEvent(msg);
    return true;
}

bool AlLayerRender::onSave(Message *m) {
    std::string path = getString("output_path");
    if (path.empty()) {
        if (onSaveListener) {
            onSaveListener(Hw::FAILED.code, "Failed", path.c_str());
        }
        return true;
    }
    auto output = mCanvas.getOutput();
    size_t size = static_cast<size_t>(output->getWidth() * output->getHeight() * 4);
    AlBuffer *buf = AlBuffer::alloc(size);
    mCanvas.read(buf);
    HwBitmapFactory::save(output->getWidth(), output->getHeight(), buf, path);
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
    auto model = _getCanvas();
    if (model->getWidth() > 0 && model->getHeight() > 0) {
        return;
    }
    model->set(size.width, size.height, 0);
    mCanvas.update(model->getWidth(), model->getHeight(), model->getColor(),
                   texAllocator);
    Message *msg = new Message(EVENT_LAYER_MEASURE_CANVAS_SIZE, nullptr,
                               Message::QUEUE_MODE_FIRST_ALWAYS);
    msg->arg1 = model->getWidth();
    msg->arg2 = model->getHeight();
    postEvent(msg);
}

AlImageCanvasModel *AlLayerRender::_getCanvas() {
    return dynamic_cast<AlImageCanvasModel *>(getObject("canvas"));
}

void AlLayerRender::_draw(AlImageLayerDrawModel *description) {
    mCanvas.draw(description);
}

AlGLContext *AlLayerRender::_getContext() {
    return dynamic_cast<AlGLContext *>(getObject("AL_CONTEXT"));
}

void AlLayerRender::setOnSaveListener(AlLayerRender::OnSaveListener listener) {
    this->onSaveListener = listener;
}