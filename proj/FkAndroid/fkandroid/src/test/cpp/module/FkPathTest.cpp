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

FkDoubleVec2 normalize(FkDoubleVec2 Vin) {
    FkDoubleVec2 result(0, 0);
    result = Vin;
    double len = std::sqrt(result.x * result.x  + result.y * result.y);
    result = result / len;

    return result;
}

struct FkMesh {
    FkMesh()
    {
        vecPoint.clear();
        vecTriangles.clear();
    }

    void clear()
    {
        vecPoint.clear();
        vecTriangles.clear();
    }

    std::vector<FkDoubleVec2> vecPoint;
    std::vector<int> vecTriangles;
};

void curveTriangulation(float width, std::vector<FkDoubleVec2> &points, std::vector<FkMesh> &vecMesh) {
    FkDoubleVec2 front(0, 0);
    FkDoubleVec2 current(0, 0);
    FkDoubleVec2 next(0, 0);

    FkDoubleVec2 outP1(0, 0);
    FkDoubleVec2 outP2(0, 0);
    FkDoubleVec2 outP3(0, 0);
    FkDoubleVec2 outP4(0, 0);

    FkDoubleVec2 dir(0, 0);
    FkDoubleVec2 dir1(0, 0);
    FkDoubleVec2 dir2(0, 0);

    int count = points.size();

    FkMesh mesh;
    FkMesh mesh1;
    FkMesh mesh2;
    for(int i = 0; i < count; i++) {
        //第一个
        if(i == 0)
        {
            current = FkDoubleVec2(points[i].x ,points[i].y);
            front = FkDoubleVec2(points[i].x ,points[i].y);
            next = FkDoubleVec2(points[i + 1].x ,points[i + 1].y);
        }
        else if(i == (count - 1))
        {//最后一个
            current = FkDoubleVec2(points[i].x ,points[i].y);
            front = FkDoubleVec2(points[i - 1].x ,points[i - 1].y);
            next = FkDoubleVec2(points[i].x ,points[i].y);
        }
        else
        {
            current = FkDoubleVec2(points[i].x ,points[i].y);
            front = FkDoubleVec2(points[i - 1].x ,points[i - 1].y);
            next = FkDoubleVec2(points[i + 1].x ,points[i + 1].y);
        }

        if( abs(next.x - current.x)<=0.000001 && abs(next.y - current.y)<=0.000001)
        {
            dir = normalize(current - front);
        }
        else if( abs(front.x - current.x)<=0.000001 && abs(front.y - current.y) <=0.000001)
        {
            dir = normalize(next - current);
        }
        else
        {
            dir1 = normalize(current - front);
            dir2 = normalize(next - current);
            dir = normalize( dir1 + dir2 );
        }

        FkDoubleVec2 normal = FkDoubleVec2(-dir.y, dir.x);
        if(i == 0)
        {
            FkDoubleVec2 tmp = current + normal * width * 1 * 1/2;
            outP1 = FkDoubleVec2(tmp.x, tmp.y);
            tmp = current + normal * width * -1 * 1/2;
            outP2 = FkDoubleVec2(tmp.x, tmp.y);
            continue;
        }
        else
        {
            float len = 0;
            FkDoubleVec2 vp = current - front;
            len = std::sqrt(vp.x * vp.x  + vp.y * vp.y);

            //计算拐角点
            FkDoubleVec2 tmp = current + normal * width * 1 * 1/2;
            outP3 = FkDoubleVec2(tmp.x, tmp.y);
            tmp = current + normal * width * -1 * 1/2;
            outP4 = FkDoubleVec2(tmp.x, tmp.y);
            mesh.clear();

            //顶点坐标
            mesh.vecPoint.push_back(outP1);
            mesh.vecPoint.push_back(outP2);
            mesh.vecPoint.push_back(outP3);
            mesh.vecPoint.push_back(outP4);

            //面索引数据
            mesh.vecTriangles.push_back(0);
            mesh.vecTriangles.push_back(1);
            mesh.vecTriangles.push_back(2);
            mesh.vecTriangles.push_back(1);
            mesh.vecTriangles.push_back(3);
            mesh.vecTriangles.push_back(2);


            vecMesh.emplace_back(mesh);

            outP1 = outP3;
            outP2 = outP4;
        }
    }
}

TEST(FkPathTest, CatmullRomAndMesh) {
    float strokeWidth = 2;
    auto path = std::make_shared<FkPathCompo>(std::make_shared<FkCatmullRomPath>(strokeWidth));
    path->addPoint(20, 20);
    path->addPoint(120, 170);
    path->addPoint(200, 180);
    path->addPoint(340, 340);
    path->addPoint(50, 420);
    path->finish();
    std::vector<FkDoubleVec2> points;
    EXPECT_TRUE(path->readPoints(points) > 0);

    std::vector<FkMesh> vecMesh;
    curveTriangulation(strokeWidth, points, vecMesh);
    FkLogI("aliminabc", "Mesh count %d", vecMesh.size());

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
    for (auto &mesh : vecMesh) {
        for (int i = 0; i < mesh.vecTriangles.size() / 3; ++i) {
            auto p0 = mesh.vecPoint[mesh.vecTriangles[i * 3]];
            auto p1 = mesh.vecPoint[mesh.vecTriangles[i * 3 + 1]];
            auto p2 = mesh.vecPoint[mesh.vecTriangles[i * 3 + 2]];
            canvas->drawLine(SkPoint::Make(p0.x, p0.y), SkPoint::Make(p1.x, p1.y), paint);
            canvas->drawLine(SkPoint::Make(p1.x, p1.y), SkPoint::Make(p2.x, p2.y), paint);
            canvas->drawLine(SkPoint::Make(p2.x, p2.y), SkPoint::Make(p0.x, p0.y), paint);
        }
    }
    canvas->flush();
    FkBitmap::write(FK_ANDROID_TEST_TEMP_FILE, FkImage::Format::kPNG, buf, size, 80);
}