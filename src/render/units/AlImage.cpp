/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlImage.h"
#include "HwBitmapFactory.h"
#include "HwTexture.h"

AlImage::AlImage(string alias) : Unit(alias) {
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&AlImage::onPrepare));
    registerEvent(EVENT_AIMAGE_NEW_LAYER, reinterpret_cast<EventFunc>(&AlImage::onNewLayer));
}

AlImage::~AlImage() {

}

bool AlImage::eventRelease(Message *msg) {
    if (texAllocator) {
        delete texAllocator;
        texAllocator = nullptr;
    }
    return true;
}

bool AlImage::onPrepare(Message *msg) {
    texAllocator = new TextureAllocator();
    return true;
}

bool AlImage::onNewLayer(Message *msg) {
    std::string path = getString("layer");
    HwBitmap *bmp = HwBitmapFactory::decodeFile(path);
    if (nullptr == bmp) {
        Logcat::e("AlImage", "%s(%d): decode %s failed", __FUNCTION__, __LINE__, path.c_str());
        return true;
    }
    tex = texAllocator->alloc(bmp->getPixels(),
                              bmp->getWidth(),
                              bmp->getHeight(),
                              GL_RGBA);
    delete bmp;
    if (GL_NONE != tex) {
        Message *m = new Message(EVENT_RENDER_FILTER, nullptr);
        m->obj = HwTexture::wrap(tex->target(), tex->texId(),
                                 tex->getWidth(),
                                 tex->getHeight(),
                                 tex->fmt());
        postEvent(m);
    }
    return true;
}
