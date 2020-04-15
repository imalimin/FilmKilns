/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

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
    _countOfByte += tex->countOfByte();
    return tex;
}

bool AlTexAllocator::recycle(HwAbsTexture **tex) {
    if (nullptr == tex || nullptr == *tex) {
        return false;
    }
    for (auto itr = textures.cbegin(); itr != textures.cend(); itr++) {
        auto *it = *itr;
        if (it && *tex == it) {
            Logcat::i(TAG, "%s(%d): %d", __FUNCTION__, __LINE__, it->texId());
            _countOfByte -= it->countOfByte();
            delete *tex;
            *tex = nullptr;
            textures.erase(itr);
            return true;
        }
    }
    return false;
}

void AlTexAllocator::clear() {
    for (auto it:textures) {
        if (nullptr != it) {
            delete it;
        }
    }
    textures.clear();
    _countOfByte = 0;
}

size_t AlTexAllocator::countOfTex() {
    return textures.size();
}

bool AlTexAllocator::empty() {
    return 0 == countOfTex();
}

int64_t AlTexAllocator::countOfByte() {
    return _countOfByte;
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
