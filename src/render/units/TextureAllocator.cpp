//
// Created by mingyi.li on 2018/12/27.
//

#include "../include/TextureAllocator.h"
#include "ObjectBox.h"
#include "log.h"
#include "../include/HwTexture.h"

TextureAllocator::TextureAllocator() {
}

TextureAllocator::~TextureAllocator() {
    clear();
}

HwAbsTexture *TextureAllocator::alloc() {
    HwAbsTexture *tex = HwTexture::alloc();
    if (nullptr == tex) {
        return nullptr;
    }
    textures.push_back(tex);
    return tex;
}

HwAbsTexture *TextureAllocator::alloc(int width, int height, uint32_t fmt) {
    return alloc(nullptr, width, height, fmt);
}

HwAbsTexture *TextureAllocator::alloc(uint8_t *rgba, int width, int height, uint32_t fmt) {
    HwAbsTexture *tex = alloc();
    if (nullptr == tex) {
        return nullptr;
    }
    HwBuffer *buf = HwBuffer::wrap(rgba, width * height * 4);
    tex->update(buf, width, height, fmt);
    delete buf;
    return tex;
}

void TextureAllocator::recycle(HwAbsTexture **tex) {
    if (nullptr == tex || nullptr == *tex) {
        return;
    }
    for (auto itr = textures.cbegin(); itr != textures.cend(); itr++) {
        if (nullptr != *itr && *tex == *itr) {
            LOGI("TextureAllocator %s %d", __func__, (*tex)->texId());
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
