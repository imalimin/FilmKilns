/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "gtest/gtest.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkPoint.h"
#include "FkSize.h"
#include "FkBuffer.h"
#include "FkBitmap.h"
#include "FkCatmullRomPath.h"
#include "FkPathCompo.h"

#define FK_ANDROID_TEST_TEMP_FILE "/storage/emulated/0/Android/data/com.alimin.fk.test/cache/000000.png"

static SkPoint calcCatmullRomPoint(SkPoint &p0, SkPoint &p1, SkPoint &p2, SkPoint &p3, float t) {
    int x = p0.x() * (-0.5 * t * t * t + t * t - 0.5 * t) +
            p1.x() * (1.5 * t * t * t - 2.5 * t * t + 1.0) +
            p2.x() * (-1.5 * t * t * t + 2.0 * t * t + 0.5 * t) +
            p3.x() * (0.5 * t * t * t - 0.5 * t * t);
    int y = p0.y() * (-0.5 * t * t * t + t * t - 0.5 * t) +
            p1.y() * (1.5 * t * t * t - 2.5 * t * t + 1.0) +
            p2.y() * (-1.5 * t * t * t + 2.0 * t * t + 0.5 * t) +
            p3.y() * (0.5 * t * t * t - 0.5 * t * t);
    return SkPoint::Make(x, y);
}

static std::vector<SkPoint> calcPath(std::vector<SkPoint> points, float avgPointDistance) {
    SkPoint start = SkPoint::Make(points[0].x() * 2 - points[1].x(),
                                  points[0].y() * 2 - points[1].y());
    SkPoint end = SkPoint::Make(points[points.size()-1].x() * 2 - points[points.size()-2].x(),
                                  points[points.size()-1].y() * 2 - points[points.size()-2].y());
    points.emplace_back(end);
    std::vector<SkPoint> path;
    for (int i = 0; i < points.size() - 2; ++i) {
        auto distance = SkPoint::Distance(points[i], points[i + 1]);
        int count = distance / avgPointDistance;
        path.emplace_back(points[i]);
        for (int j = 1; j < count; ++j) {
            float t = j / (float) count;
            auto p = calcCatmullRomPoint(start, points[i], points[i + 1], points[i + 2], t);
            path.emplace_back(p);
        }
        path.emplace_back(points[i + 1]);
        start = points[i];
    }
    return path;
}

TEST(FkPathTest, CatmullRom) {
    auto path = std::make_shared<FkPathCompo>(std::make_shared<FkCatmullRomPath>(10));
    path->addPoint(20, 20);
    path->addPoint(120, 170);
    path->addPoint(200, 180);
    path->addPoint(340, 340);
    path->addPoint(50, 420);
    path->finish();
    std::vector<FkDoubleVec2> points;
    EXPECT_TRUE(path->readPoints(points) > 0);

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(2);
    paint.setStrokeCap(SkPaint::kRound_Cap);
    paint.setStrokeJoin(SkPaint::kRound_Join);

    FkSize size(480, 720);
    SkImageInfo info = SkImageInfo::Make({size.getWidth(), size.getHeight()}, kRGBA_8888_SkColorType, kUnpremul_SkAlphaType, nullptr);
    auto buf = FkBuffer::alloc(info.computeMinByteSize());
    memset(buf->data(), 0, buf->capacity());
    auto canvas = SkCanvas::MakeRasterDirect(info, buf->data(), info.minRowBytes());
    paint.setColor(0xffffffff);
    for (int i = 0; i < points.size() - 1; ++i) {
        canvas->drawPoint(SkPoint::Make(points[i].x, points[i].y), paint);
    }

//    paint.setColor(0xff349a45);
//    for (int i = 0; i < points.size() - 1; ++i) {
//        canvas->drawPoint(points[i], paint);
//    }
    canvas->flush();
//    FkBitmap::write(FK_ANDROID_TEST_TEMP_FILE, FkImage::Format::kPNG, buf, size, 80);
}