//
// Created by mingyi.li on 2018/12/27.
//

#include "AlTexAllocator.h"
#include "ObjectBox.h"
#include "log.h"
#include "HwTexture.h"

#define TAG "TextureAllocator"

AlTexAllocator::AlTexAllocator() {
}

AlTexAllocator::~AlTexAllocator() {
    clear();
}

HwAbsTexture *AlTexAllocator::alloc() {
    AlTexDescription desc;
    return alloc(desc);
}

HwAbsTexture *AlTexAllocator::alloc(AlTexDescription &desc, AlBuffer *buf) {
    HwAbsTexture *tex = HwTexture::alloc(desc);
    if (nullptr == tex) {
        return nullptr;
    }
    textures.push_back(tex);
    if (desc.size.width > 0 && desc.size.height > 0) {
        tex->update(buf, desc.size.width, desc.size.height, desc.fmt);
    }
    return tex;
}

void AlTexAllocator::recycle(HwAbsTexture **tex) {
    if (nullptr == tex || nullptr == *tex) {
        return;
    }
    for (auto itr = textures.cbegin(); itr != textures.cend(); itr++) {
        auto *it = *itr;
        if (it && *tex == it) {
            Logcat::i(TAG, "%s(%d): %d", __FUNCTION__, __LINE__, it->texId());
            delete *tex;
            *tex = nullptr;
            textures.erase(itr);
            break;
        }
    }
}

void AlTexAllocator::clear() {
    for (auto it:textures) {
        if (nullptr != it) {
            delete it;
        }
    }
    textures.clear();
}

//HwAbsTexture *AlTexAllocator::alloc(AlBitmap *bmp) {
//    AlTexDescription desc;
//    desc.size.width = bmp->getWidth();
//    desc.size.height = bmp->getHeight();
//    desc.fmt = GL_RGBA;
//    AlBuffer *buf = AlBuffer::wrap(bmp->getPixels(), bmp->getByteSize());
//    auto *tex = alloc(desc, buf);
//    delete buf;
//    return tex;
//}
