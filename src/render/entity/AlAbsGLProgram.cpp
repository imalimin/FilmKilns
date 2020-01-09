//
// Created by mingyi.li on 2020-01-09.
//

#include "AlAbsGLProgram.h"
#include "Logcat.h"

#include <vector>

#define TAG "AlAbsGLProgram"

AlAbsGLProgram::AlAbsGLProgram(std::string *v, std::string *f) : Object() {
    this->program = _createProgram(v, f);
}

AlAbsGLProgram::~AlAbsGLProgram() {
    if (GL_NONE != program) {
        glDeleteProgram(program);
        program = GL_NONE;
    }
}

uint32_t AlAbsGLProgram::_createShader(uint32_t type, string *shader) {
    GLuint shaderId = glCreateShader(type);
    if (shaderId == 0) {
        Logcat::e(TAG, "%s(%d) Create Shader Failed: %d", __FUNCTION__, __LINE__, glGetError());
        return 0;
    }
    //加载Shader代码
    const char *s = shader->c_str();
    glShaderSource(shaderId, 1, &s, 0);
    //编译Shader
    glCompileShader(shaderId);
    GLint status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (GL_TRUE != status) {
#ifdef GL_DEBUG
        GLint len;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> log(static_cast<unsigned int>(len));
        glGetShaderInfoLog(shaderId, len, nullptr, log.data());
        string str(begin(log), end(log));
        Logcat::e(TAG, "%s(%d) createShader(%d) error:%s >>>>>>>>> Source: %s",
                  __FUNCTION__, __LINE__,
                  type,
                  str.c_str(),
                  shader->c_str());
#endif
        glDeleteShader(shaderId);
        shaderId = GL_NONE;
    }
    return shaderId;
}

uint32_t AlAbsGLProgram::_createProgram(string *vertex, string *fragment) {
    GLuint program = glCreateProgram();
    if (program == GL_NONE) {
        Logcat::e(TAG, "%s(%d) Create program failed: %d", __FUNCTION__, __LINE__, glGetError());
        return GL_NONE;
    }
    GLuint vertexShader = _createShader(GL_VERTEX_SHADER, vertex);
    GLuint fragmentShader = _createShader(GL_FRAGMENT_SHADER, fragment);
    if (GL_NONE == vertexShader || GL_NONE == fragmentShader) {
        return GL_NONE;
    }
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    bind();
    return program;
}

void AlAbsGLProgram::bind() {
    if (!_checkProgram()) return;
    glUseProgram(program);
}

void AlAbsGLProgram::unbind() {
    glUseProgram(GL_NONE);
}

void AlAbsGLProgram::setUniform1f(int32_t location, float value) {
    glUniform1f(location, value);
}

void AlAbsGLProgram::setUniform2fv(int32_t location, AlVec2 &vec2) {
    auto *value = new float[2]{vec2.x, vec2.y};
    glUniform2fv(location, 1, value);
    delete[] value;
}

void AlAbsGLProgram::setUniform2fv(int32_t location, int32_t count, float *array) {
    glUniform2fv(location, count, array);
}

void AlAbsGLProgram::setUniformMatrix4fv(int32_t location, float *value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void AlAbsGLProgram::setUniform1i(int32_t location, int32_t value) {
    glUniform1i(location, value);
}

int32_t AlAbsGLProgram::getAttribLocation(string name) {
    if (!_checkProgram()) return GL_NONE;
    return glGetAttribLocation(program, name.c_str());
}

int32_t AlAbsGLProgram::getUniformLocation(string name) {
    if (!_checkProgram()) return GL_NONE;
    return glGetUniformLocation(program, name.c_str());
}

bool AlAbsGLProgram::_checkProgram() {
    return GL_NONE != program;
}

int32_t AlAbsGLProgram::getError() {
    GLenum error = glGetError();
    if (GL_NO_ERROR != error) {
        Logcat::i(TAG, "%s(%d) OpenGL ES throw an error: %x", __FUNCTION__, __LINE__, error);
    }
    return error;
}
