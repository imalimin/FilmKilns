/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICPROGRAM_H
#define FK_GRAPHIC_FKGRAPHICPROGRAM_H

#include "FkSource.h"
#include "FkDefinition.h"
#include "FkVec2.h"

FK_CLASS FkProgramDescription FK_EXTEND FkObject {
public:
    AFK_ENUM kType : int32_t {
        NONE = 0,
        MATRIX,
    };
public:
    FkProgramDescription();

    FkProgramDescription(kType type);

    FkProgramDescription(const FkProgramDescription &o);

    virtual ~FkProgramDescription();

public:
    kType type = kType::NONE;
};

FK_ABS_CLASS FkGraphicProgram FK_EXTEND FkSource {
public:
    FkGraphicProgram(const FkProgramDescription &desc);

    FkGraphicProgram(const FkGraphicProgram &o) = delete;

    virtual ~FkGraphicProgram();

    virtual std::string getVertex() = 0;

    virtual std::string getFragment() = 0;

    virtual FkResult create() override;

    virtual void destroy() override;

    virtual size_t size() override;

    virtual void bind();

    virtual void unbind();

    int32_t getAttribLocation(string name);

    int32_t getUniformLocation(string name);

    void setUniform1f(int32_t location, float value);

    void setUniform2fv(int32_t location, FkVec2 &vec2);

    void setUniform2fv(int32_t location, int32_t count, float *array);

    void setUniform4fv(int32_t location, int32_t count, float *array);

    void setUniformMatrix4fv(int32_t location, float *value);

    void setUniform1i(int32_t location, int32_t value);

private:
    uint32_t _createShader(uint32_t type, std::string &shader);

    bool _checkProgram();

public:
    FkProgramDescription desc;
    uint32_t program = 0;
};

class FkGraphicMatProgram;

FK_CLASS FkGraphicProgramAllocator FK_EXTEND FkSourceAllocator<
        FkGraphicProgram,
        FkProgramDescription> {
public:
    FkGraphicProgramAllocator();

    FkGraphicProgramAllocator(const FkProgramDescription &o) = delete;

    ~FkGraphicProgramAllocator();

    virtual FkGraphicProgram *delegateAlloc(FkProgramDescription &desc) override;

    virtual bool delegateEquals(FkProgramDescription &desc, FkGraphicProgram *tex) override;

};


#endif //FK_GRAPHIC_FKGRAPHICPROGRAM_H
