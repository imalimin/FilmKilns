/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-09-01 00:03:21
*/

#ifndef FK_GRAPHIC_FKTEXTURECHARMAP_H
#define FK_GRAPHIC_FKTEXTURECHARMAP_H

#include "FkObject.h"
#include "FkColor.h"
#include "FkRect.h"
#include "FkGraphicTexture.h"
#include "FkFloatVec2.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include <unordered_map>

FK_SUPER_CLASS(FkTextureCharMap, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkTextureCharMap)

public:
    FkTextureCharMap(int32_t fontSize, const FkColor &color);

    FkTextureCharMap(const FkTextureCharMap &o) = delete;

    virtual ~FkTextureCharMap();

    FkFloatRect find(char c);

    std::shared_ptr<FkGraphicTexture> getTexture();

private:
    FkResult _createTextTexture();

    void _drawChar(char c, const FkFloatRect &rect, SkCanvas &canvas);

    void _drawCharRange(char start, char end, FkFloatVec2 &pos, float &maxHeight, SkCanvas &canvas);

    SkRect _measureChar(char c);

private:
    int32_t fontSize;
    FkColor color;
    SkFont font;
    SkPaint paint;

    std::shared_ptr<FkGraphicAllocator> allocator = nullptr;
    std::shared_ptr<FkGraphicTexture> mCharsTex = nullptr;
    std::unordered_map<char, FkFloatRect> map;
};

#endif //FK_GRAPHIC_FKTEXTURECHARMAP_H