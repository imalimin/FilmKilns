//
// Created by mingyi.li on 2020-01-09.
//

#ifndef HWVC_ANDROID_ALABSGLPROGRAM_H
#define HWVC_ANDROID_ALABSGLPROGRAM_H

#include "Object.h"
#include "AlVec2.h"
#include "HwAbsTexture.h"

al_class(AlAbsGLProgram) {
public:
    AlAbsGLProgram(std::string *v, std::string *f);

    AlAbsGLProgram(const AlAbsGLProgram &o) : Object() {};

    virtual ~AlAbsGLProgram();

    virtual void draw(HwAbsTexture *tex) = 0;

    void bind();

    void unbind();

    int32_t getAttribLocation(string name);

    int32_t getUniformLocation(string name);

    void setUniform1f(int32_t location, float value);

    void setUniform2fv(int32_t location, AlVec2 &vec2);

    void setUniform2fv(int32_t location, int32_t count, float *array);

    void setUniformMatrix4fv(int32_t location, float *value);

    void setUniform1i(int32_t location, int32_t value);

private:
    uint32_t _createShader(uint32_t type, string *shader);

    uint32_t _createProgram(string *vertex, string *fragment);

    bool _checkProgram();

protected:
    uint32_t program = 0;
};


#endif //HWVC_ANDROID_ALABSGLPROGRAM_H
