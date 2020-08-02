/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlNorTexDrawer.h"
#include "AlTexManager.h"
#include "HwFBObject.h"

AlNorTexDrawer::AlNorTexDrawer() : Object() {

}

AlNorTexDrawer::~AlNorTexDrawer() {
    delete fbo;
    fbo = nullptr;

    if (nv12Filter) {
        delete nv12Filter;
        nv12Filter = nullptr;
    }
    if (yv12Filter) {
        delete yv12Filter;
        yv12Filter = nullptr;
    }
    if (v) {
        AlTexManager::instance()->recycle(&v);
    }
    if (u) {
        AlTexManager::instance()->recycle(&u);
    }
    if (y) {
        AlTexManager::instance()->recycle(&y);
    }
}

void AlNorTexDrawer::draw(AlBuffer *buf, AlSize *size, AlColorFormat fmt, HwAbsTexture *dst) {
    if (AlColorFormat::YV12 == fmt) {
        _drawYUV420P(buf, size, dst);
    } else if (AlColorFormat::NV12 == fmt) {
        _drawYUV420SP(buf, size, dst);
    } else if (AlColorFormat::RGBA == fmt) {
        _drawRGBA(buf, size, dst);
    } else {
        glViewport(0, 0, dst->getWidth(), dst->getHeight());
        if (nullptr == fbo) {
            fbo = HwFBObject::alloc();
        }
        fbo->bindTex(dst);
        fbo->bind();
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fbo->unbind();
    }
}

void AlNorTexDrawer::_drawYUV420P(AlBuffer *buf, AlSize *size, HwAbsTexture *dst) {
    if (nullptr == y) {
        y = AlTexManager::instance()->alloc();
    }
    if (nullptr == u) {
        u = AlTexManager::instance()->alloc();
    }
    if (nullptr == v) {
        v = AlTexManager::instance()->alloc();
    }
    if (nullptr == yv12Filter) {
        yv12Filter = new HwYV122RGBAFilter();
        yv12Filter->prepare();
    }

    int len = size->width * size->height;
    AlBuffer *tmp = AlBuffer::wrap(buf->data(), len);
    y->update(tmp, size->width, size->height, GL_LUMINANCE);
    delete tmp;

    tmp = AlBuffer::wrap(buf->data() + len, len / 4);
    u->update(tmp, size->width / 2, size->height / 2, GL_LUMINANCE);
    delete tmp;
    tmp = AlBuffer::wrap(buf->data() + len + len / 4, len / 4);
    v->update(tmp, size->width / 2, size->height / 2, GL_LUMINANCE);
    delete tmp;

    glViewport(0, 0, size->width, size->height);
    yv12Filter->draw(y, u, v, dst);
}

void AlNorTexDrawer::_drawYUV420SP(AlBuffer *buf, AlSize *size, HwAbsTexture *dst) {
    if (nullptr == y) {
        y = AlTexManager::instance()->alloc();
    }
    if (nullptr == u) {
        u = AlTexManager::instance()->alloc();
    }
    if (nullptr == v) {
        v = AlTexManager::instance()->alloc();
    }
    if (nullptr == nv12Filter) {
        nv12Filter = new AlNV12ToRGBAFilter();
        nv12Filter->prepare();
    }

    int len = size->width * size->height;
    AlBuffer *tmp = AlBuffer::wrap(buf->data(), len);
    y->update(tmp, size->width, size->height, GL_LUMINANCE);
    delete tmp;

    tmp = AlBuffer::wrap(buf->data() + len, len / 2);
    u->update(tmp, size->width / 2, size->height / 2, GL_LUMINANCE_ALPHA);
    delete tmp;

    glViewport(0, 0, size->width, size->height);
    nv12Filter->draw(y, u, dst);
}

void AlNorTexDrawer::_drawRGBA(AlBuffer *buf, AlSize *size, HwAbsTexture *dst) {
    dst->update(buf, size->width, size->height);
}