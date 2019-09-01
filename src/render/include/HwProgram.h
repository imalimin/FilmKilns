/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWPROGRAM_H
#define HWVC_ANDROID_HWPROGRAM_H

#include "Object.h"
#include "HwAbsTexture.h"
#include "HwMatrix.h"

#define HW_SHADER(...) #__VA_ARGS__

class HwProgram : public Object {
public:
    static HwProgram *create(string *vertex, string *fragment);

public:
    virtual ~HwProgram();

    void draw(HwAbsTexture *tex);

    void updateLocation(float *texCoordinate, float *position);

    int32_t getAttribLocation(string name);

    int32_t getUniformLocation(string name);

    void setUniform1f(int32_t location, float value);

    void setUniformMatrix4fv(int32_t location, float *value);

    void updateMatrix(HwMatrix *matrix);

    void bind();

    void unbind();

private:
    HwProgram(string *vertex, string *fragment);

    void updateVBOs();

    uint32_t createVBOs();

    uint32_t createShader(uint32_t type, string *shader);

    uint32_t createProgram(string *vertex, string *fragment);

    void enableVertex(uint32_t posLoc, uint32_t texLoc);

private:
    //每个点占多少字节
    const int32_t HW_SIZE_OF_VERTEX = 4;
    //一个顶点坐标包含几个点
    const int32_t HW_COUNT_PER_VERTEX = 2;
    //所有顶点坐标总共多少字节
    const int32_t HW_VERTEX_BYTE_SIZE = HW_SIZE_OF_VERTEX * HW_COUNT_PER_VERTEX * 4;

private:
    uint32_t program = 0;
    int32_t uTextureLocation = 0;
    int32_t aPositionLocation = 0;
    int32_t aTextureCoordinateLocation = 0;
    int32_t uTextureMatrix = 0;
    float position[8] = {0.0f};
    float texCoordinate[8] = {0.0f};
    HwMatrix matrix;
    bool requestUpdateLocation = false;
    uint32_t vbo = 0;
    int rotation = 0;
};


#endif //HWVC_ANDROID_HWPROGRAM_H
