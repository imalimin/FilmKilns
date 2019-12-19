/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "../include/Image.h"
#include "ObjectBox.h"
#include "../include/HwTexture.h"
#include "Logcat.h"

Image::Image(string alias) : Unit(alias) {
    registerEvent(EVENT_IMAGE_SHOW, reinterpret_cast<EventFunc>(&Image::eventShow));
    registerEvent(EVENT_COMMON_INVALIDATE, reinterpret_cast<EventFunc>(&Image::eventInvalidate));
}

Image::~Image() {
}

bool Image::onDestroy(AlMessage *msg) {
    Logcat::i("HWVC", "Image::onDestroy");
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    if (hwBitmap) {
        delete hwBitmap;
        hwBitmap = nullptr;
    }
    return true;
}

void Image::show(string path) {
    if (!decode(path)) {
        return;
    }
    tex = texAllocator->alloc(hwBitmap->getPixels(),
                              hwBitmap->getWidth(),
                              hwBitmap->getHeight(),
                              GL_RGBA);
    eventInvalidate(nullptr);
}

bool Image::decode(string path) {
    if (hwBitmap) {//HwBitmap暂时不支持复用，所以先删除
        delete[]hwBitmap;
        hwBitmap = nullptr;
    }
    hwBitmap = HwBitmapFactory::decodeFile(path);
    if (!hwBitmap) {
        Logcat::i("HWVC", "Image decode %s failed", path.c_str());
        return false;
    }
    Logcat::i("HWVC", "Image decode(%d x %d) %s",
              hwBitmap->getWidth(),
              hwBitmap->getHeight(),
              path.c_str());
    return true;
}

bool Image::onCreate(AlMessage *msg) {
    texAllocator = new TextureAllocator();
    return true;
}

bool Image::eventShow(AlMessage *msg) {
    if (msg->obj) {
        string *path = static_cast<string *>(msg->getObj<ObjectBox *>()->ptr);
        show(*path);
        delete path;
    }
    return true;
}

bool Image::eventInvalidate(AlMessage *m) {
    if (GL_NONE != tex) {
        AlMessage *msg = AlMessage::obtain(EVENT_RENDER_FILTER);
        msg->obj = HwTexture::wrap(tex->target(), tex->texId(),
                                   tex->getWidth(),
                                   tex->getHeight(),
                                   tex->fmt());
        postEvent(msg);
    }
    return true;
}