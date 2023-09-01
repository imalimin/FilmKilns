/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-09-01 00:03:21
*/

#include "FkTextureCharMap.h"
#include "FkGLDefinition.h"
#include "include/core/SkImageEncoder.h"

FK_IMPL_CLASS_TYPE(FkTextureCharMap, FkObject)

FkTextureCharMap::FkTextureCharMap(int32_t fontSize, const FkColor &color)
        : FkObject(), fontSize(fontSize), color(color) {
    allocator = std::make_shared<FkGraphicAllocator>(256 * 1024 * 1024);
    FkTexDescription desc(GL_TEXTURE_2D);
    desc.fmt = FkColor::kFormat::RGBA;
    desc.size = FkSize(256, 256);
    mCharsTex = allocator->alloc(desc);

    font.setSize(18);
    paint.setAntiAlias(true);
    paint.setFilterQuality(kMedium_SkFilterQuality);
    paint.setColor(SK_ColorWHITE);
    paint.setStrokeWidth(3);

    _createTextTexture();
}

FkTextureCharMap::~FkTextureCharMap() {
    mCharsTex = nullptr;
    allocator->release();
    allocator = nullptr;
}

FkResult FkTextureCharMap::_createTextTexture() {
    SkBitmap bmp;
    bmp.allocN32Pixels(mCharsTex->desc.size.getWidth(), mCharsTex->desc.size.getHeight());
    SkCanvas canvas(bmp);
    canvas.drawColor(SK_ColorTRANSPARENT);

    float maxHeight = 0;
    FkFloatVec2 pos(0, 20);
    _drawCharRange('0', '9', pos, maxHeight, canvas);
    _drawCharRange('A', 'Z', pos, maxHeight, canvas);
    _drawCharRange('a', 'z', pos, maxHeight, canvas);
    _drawCharRange('.', '.', pos, maxHeight, canvas);

    auto encodedData = SkEncodeBitmap(bmp, SkEncodedImageFormat::kPNG, 80);
    if (encodedData != nullptr) {
        FILE *f = fopen("/sdcard/Android/data/com.demo.live/cache/char_map.png", "wb");
        if (f) {
            fwrite(encodedData->data(), 1, encodedData->size(), f);
            fclose(f);
            return FK_OK;
        }
        return FK_IO_FAIL;
    }
    return FK_OK;
}

void FkTextureCharMap::_drawChar(char c, const FkFloatRect &rect, SkCanvas &canvas) {
    canvas.drawSimpleText(&c, 1, SkTextEncoding::kUTF8, rect.left(), rect.top(), font, paint);
}

void FkTextureCharMap::_drawCharRange(char start, char end, FkFloatVec2 &pos, float &maxHeight, SkCanvas &canvas) {
    for (char i = start; i <= end; ++i) {
        SkRect bounds = _measureChar(i);
        if (pos.x + bounds.width() >= mCharsTex->desc.size.getWidth()) {
            pos = FkFloatVec2(0, pos.y + maxHeight);
            maxHeight = 0;
        }
        FkFloatRect rect(pos.x, pos.y,
                         pos.x + bounds.width(), pos.y + bounds.height());
        pos.x += bounds.width();
        maxHeight = std::max<float>(maxHeight, bounds.height());
        _drawChar(i, rect, canvas);
    }
}

SkRect FkTextureCharMap::_measureChar(char c) {
    SkRect bounds;
    font.measureText(&c, 1, SkTextEncoding::kUTF8, &bounds);
    return bounds;
}

FkFloatRect FkTextureCharMap::find(char c) {
    auto itr = map.find(c);
    if (itr == map.end()) {
        return {0, 0, 0, 0};
    }
    return itr->second;
}

std::shared_ptr<FkGraphicTexture> FkTextureCharMap::getTexture() {
    return mCharsTex;
}