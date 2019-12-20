//
// Created by mingyi.li on 2018/12/27.
//

#include "TextureAllocator.h"
#include "ObjectBox.h"
#include "log.h"
#include "HwTexture.h"

#define TAG "TextureAllocator"

TextureAllocator::TextureAllocator() {
}

TextureAllocator::~TextureAllocator() {
    clear();
}

HwAbsTexture *TextureAllocator::alloc() {
    AlTexDescription desc;
    return alloc(desc);
}

HwAbsTexture *TextureAllocator::alloc(AlTexDescription &desc, AlBuffer *buf) {
    HwAbsTexture *tex = HwTexture::alloc(desc);
    if (nullptr == tex) {
        return nullptr;
    }
    textures.push_back(tex);
    if (desc.size.width > 0 && desc.size.height > 0) {
        HwBuffer *bufTmp = nullptr;
        if (buf) {
            bufTmp = HwBuffer::wrap(buf->data(), buf->size());
        }
        tex->update(bufTmp, desc.size.width, desc.size.height, desc.fmt);
        delete bufTmp;
    }
    return tex;
}

void TextureAllocator::recycle(HwAbsTexture **tex) {
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

void TextureAllocator::clear() {
    for (auto it:textures) {
        if (nullptr != it) {
            delete it;
        }
    }
    textures.clear();
}

//HwAbsTexture *TextureAllocator::alloc(HwBitmap *bmp) {
//    AlTexDescription desc;
//    desc.size.width = bmp->getWidth();
//    desc.size.height = bmp->getHeight();
//    desc.fmt = GL_RGBA;
//    AlBuffer *buf = AlBuffer::wrap(bmp->getPixels(), bmp->getByteSize());
//    auto *tex = alloc(desc, buf);
//    delete buf;
//    return tex;
//}
