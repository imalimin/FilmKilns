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
#include "FkFloatVec2.h"
#include "FkGraphicComponent.h"
#include <vector>

FK_SUPER_CLASS(FkProgramDescription, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkProgramDescription)

public:
    FK_ENUM kType : int32_t {
        NONE = 0,
        MATRIX,
        MATRIX_V3,
        MATRIX_WITH_CANVAS_BACKGROUND,
        CANVAS_MOSAIC,
        POINT,
        PATH,
    };
public:
    FkProgramDescription();

    FkProgramDescription(kType type);

    FkProgramDescription(const FkProgramDescription &o);

    virtual ~FkProgramDescription();

public:
    kType type = kType::NONE;
    int32_t maxCountOfFragmentTexture = 1;
};

FK_SUPER_CLASS(FkGraphicProgram, FkSource) {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicProgram)

public:
    FkGraphicProgram(const FkProgramDescription &desc);

    FkGraphicProgram(const FkGraphicProgram &o) = delete;

    virtual ~FkGraphicProgram();

    virtual FkProgramDescription::kType type() = 0;

    virtual std::string getVertex() = 0;

    virtual std::string getFragment() = 0;

    virtual FkResult create() override;

    virtual void destroy() override;

    virtual size_t size() override;

    virtual void bind();

    virtual void unbind();

    virtual void clear();

    virtual FkResult addValue(std::shared_ptr<FkComponent> value);

protected:
    int32_t getAttribLocation(std::string name);

    int32_t getUniformLocation(std::string name);

    void setUniform1f(int32_t location, float value);

    void setUniform4f(int32_t location, float v0, float v1, float v2, float v3);

    void setUniform2fv(int32_t location, FkFloatVec2 &vec2);

    void setUniform2fv(int32_t location, int32_t countOfArrayPtr, float *array);

    void setUniform4fv(int32_t location, int32_t countOfArrayPtr, float *array);

    void setUniformMatrix4fv(int32_t location, float *value);

    void setUniform1i(int32_t location, int32_t value);

private:
    uint32_t _createShader(uint32_t type, std::string &shader);

    bool _checkProgram();

public:
    FkProgramDescription desc;
    uint32_t program = 0;
    std::vector<std::shared_ptr<FkComponent>> values;
};

FK_SUPER_TEMPLATE_CLASS_IMPL(FkGraphicProgramAllocator, FkSourceAllocator)<FkGraphicProgram, FkProgramDescription> {
FK_DEF_CLASS_TYPE_FUNC(FkGraphicProgramAllocator)

public:
    FkGraphicProgramAllocator(int capacity);

    FkGraphicProgramAllocator(const FkProgramDescription &o) = delete;

    ~FkGraphicProgramAllocator();

    virtual FkGraphicProgram *delegateAlloc(FkProgramDescription &desc) override;

    virtual bool delegateEquals(FkProgramDescription &desc, FkGraphicProgram *program) override;

};


#endif //FK_GRAPHIC_FKGRAPHICPROGRAM_H
