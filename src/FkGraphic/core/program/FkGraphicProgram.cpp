/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkGraphicProgram.h"
#include "FkGraphicMatProgram.h"
#include "FkCanvasMosaicProgram.h"
#include "FkGraphicPointProgram.h"
#include "FkGraphicTexComponent.h"
#include "FkVertexComponent.h"
#include "FkGLDefinition.h"
#include "FkDefinition.h"

#define TAG "FkGraphicProgram"

///+-----------------------------------------------------------+
///|                 FkProgramDescription                      |
///+-----------------------------------------------------------+

FkProgramDescription::FkProgramDescription() : FkObject() {

}

FkProgramDescription::FkProgramDescription(FkProgramDescription::kType type)
        : FkObject(), type(type) {

}

FkProgramDescription::FkProgramDescription(const FkProgramDescription &o)
        : FkObject(), type(o.type) {

}

FkProgramDescription::~FkProgramDescription() {
    type = FkProgramDescription::kType::NONE;
}

///+-----------------------------------------------------------+
///|                   FkGraphicProgram                        |
///+-----------------------------------------------------------+

FkGraphicProgram::FkGraphicProgram(const FkProgramDescription &desc) : FkSource(), desc(desc) {
    FK_MARK_SUPER
}

FkGraphicProgram::~FkGraphicProgram() {
    values.clear();
}

FkResult FkGraphicProgram::create() {
    program = glCreateProgram();
    FkAssert(GL_NONE != program, GL_NONE);
    std::string vertex = getVertex();
    std::string fragment = getFragment();
    GLuint vertexShader = _createShader(GL_VERTEX_SHADER, vertex);
    GLuint fragmentShader = _createShader(GL_FRAGMENT_SHADER, fragment);
    FkAssert(GL_NONE != vertexShader && GL_NONE != fragmentShader, GL_NONE);
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    FK_GL_CHECK(glLinkProgram(program));
    FkAssert(GL_NO_ERROR == glGetError(), FK_FAIL);
    return FK_OK;
}

void FkGraphicProgram::destroy() {
    if (GL_NONE != program) {
        FK_GL_CHECK(glDeleteProgram(program));
        program = GL_NONE;
    }
}

size_t FkGraphicProgram::size() {
    return 1;
}

void FkGraphicProgram::bind() {
    glUseProgram(program);
}

void FkGraphicProgram::unbind() {
    glUseProgram(GL_NONE);
}

void FkGraphicProgram::setUniform1f(int32_t location, float value) {
    glUniform1f(location, value);
}

void FkGraphicProgram::setUniform2fv(int32_t location, FkFloatVec2 &vec2) {
    auto *value = new float[2]{vec2.x, vec2.y};
    glUniform2fv(location, 1, value);
    delete[] value;
}

void FkGraphicProgram::setUniform2fv(int32_t location, int32_t countOfArrayPtr, float *array) {
    glUniform2fv(location, countOfArrayPtr, array);
}

void FkGraphicProgram::setUniform4fv(int32_t location, int32_t countOfArrayPtr, float *array) {
    glUniform4fv(location, countOfArrayPtr, array);
}

void FkGraphicProgram::setUniformMatrix4fv(int32_t location, float *value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void FkGraphicProgram::setUniform1i(int32_t location, int32_t value) {
    glUniform1i(location, value);
}

int32_t FkGraphicProgram::getAttribLocation(string name) {
    if (!_checkProgram()) return -1;
    return glGetAttribLocation(program, name.c_str());
}

int32_t FkGraphicProgram::getUniformLocation(string name) {
    if (!_checkProgram()) return -1;
    return glGetUniformLocation(program, name.c_str());
}

bool FkGraphicProgram::_checkProgram() {
    return GL_NONE != program;
}

uint32_t FkGraphicProgram::_createShader(uint32_t type, std::string &shader) {
    GLuint shaderId = glCreateShader(type);
    FkAssert(shaderId != 0, GL_NONE);
    //Load shader code.
    const char *str = shader.c_str();
    glShaderSource(shaderId, 1, &str, 0);
    //Compile shader.
    glCompileShader(shaderId);
    GLint status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (GL_TRUE != status) {
#ifdef __FK_DEBUG__
        GLint len;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> vec(static_cast<unsigned int>(len));
        glGetShaderInfoLog(shaderId, vec.size(), nullptr, vec.data());
        std::string log(std::begin(vec), std::end(vec));
        FkLogE(TAG, "createShader(%d) error:%s >>>>>>>>> Source: %s",
               type,
               log.c_str(),
               shader.c_str());
#endif
        glDeleteShader(shaderId);
        shaderId = GL_NONE;
    }
    FkAssert(shaderId != 0, GL_NONE);
    return shaderId;
}

FkResult FkGraphicProgram::addValue(std::shared_ptr<FkGraphicComponent> value) {
    if (nullptr == value) {
        return FK_FAIL;
    }
    values.emplace_back(value);
    return FK_OK;
}

void FkGraphicProgram::clear() {
    for (auto itr = values.rbegin(); itr != values.rend(); ++itr) {
        auto it = *itr;
        if (FK_INSTANCE_OF(it, FkGraphicTexComponent)) {
            auto pValue = Fk_POINTER_CAST(FkGraphicTexComponent, it);
            glBindTexture(pValue->tex->desc.target, GL_NONE);
        }
    }
    values.clear();
}

///+-----------------------------------------------------------+
///|               FkGraphicProgramAllocator                   |
///+-----------------------------------------------------------+

FkGraphicProgramAllocator::FkGraphicProgramAllocator()
        : FkSourceAllocator<FkGraphicProgram, FkProgramDescription>() {
    FK_MARK_SUPER
}

FkGraphicProgramAllocator::~FkGraphicProgramAllocator() {

}

FkGraphicProgram *FkGraphicProgramAllocator::delegateAlloc(FkProgramDescription &desc) {
    FkGraphicProgram *o = nullptr;
    switch (desc.type) {
        case FkProgramDescription::kType::MATRIX: {
            o = new FkGraphicMatProgram(desc);
            break;
        }
        case FkProgramDescription::kType::CANVAS_MOSAIC: {
            o = new FkCanvasMosaicProgram(desc);
            break;
        }
        case FkProgramDescription::kType::POINT:
            o = new FkGraphicPointProgram(desc);
            break;
    }
    if (o) {
        o->create();
    }
    return o;
}

bool FkGraphicProgramAllocator::delegateEquals(FkProgramDescription &desc, FkGraphicProgram *tex) {
    return desc.type == tex->desc.type;
}
