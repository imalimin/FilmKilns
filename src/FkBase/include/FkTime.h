//
// Created by lmy on 2023/10/14.
//
#pragma once

#include "FkObject.h"
#include "FkRational.h"

FK_SUPER_CLASS(FkTime, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkTime)

public:
    FK_ENUM kUnit {
        ORIGIN,
        NS,
        US,
        MS,
        SEC,
    };

public:
    static FkTime zero();

    static FkTime makeUS(int64_t us);

public:
    FkTime(int64_t value, FkRational timeBase);

    FkTime(const FkTime &o);

    virtual ~FkTime();

    int64_t get(kUnit unit);

private:
    int64_t value;
    FkRational timeBase;
};
