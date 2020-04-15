/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWPROGRAM_H
#define HWVC_ANDROID_HWPROGRAM_H

#include "AlAbsGLProgram.h"
#include "HwAbsTexture.h"
#include "AlMatrix.h"
#include "vector"

al_class_ex(HwProgram, AlAbsGLProgram) {
public:
    static void calculateFitWinVertex(float *vertex,
                                      Size texSize,
                                      Size winSize,
                                      int32_t scaleType = 0);

    static HwProgram *create(string *vertex, string *fragment);

public:
    virtual ~HwProgram();

    virtual void draw(HwAbsTexture *tex) override;

    void updateLocation(float *texCoordinate, float *position);

    void updateMatrix(AlMatrix *matrix);

private:
    HwProgram(string *vertex, string *fragment);

    HwProgram(const HwProgram &o) : AlAbsGLProgram(o) {};

    void updateVBOs();

    uint32_t createVBOs();

    void enableVertex(uint32_t posLoc, uint32_t texLoc);

private:
    //每个点占多少字节
    const int32_t HW_SIZE_OF_VERTEX = 4;
    //一个顶点坐标包含几个点
    const int32_t HW_COUNT_PER_VERTEX = 2;
    //所有顶点坐标总共多少字节
    const int32_t HW_VERTEX_BYTE_SIZE = HW_SIZE_OF_VERTEX * HW_COUNT_PER_VERTEX * 4;

private:
    int32_t uTextureLocation = -1;
    int32_t aPositionLocation = -1;
    int32_t aTextureCoordinateLocation = -1;
    int32_t uTextureMatrix = -1;
    uint32_t vbo = 0;
    float position[8] = {0.0f};
    float texCoordinate[8] = {0.0f};
    AlMatrix matrix;
    bool requestUpdateLocation = false;
};


#endif //HWVC_ANDROID_HWPROGRAM_H
