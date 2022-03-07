//
// Created by Alimin on 2021/8/6.
//

#ifndef FKANDROID_FKWITHGLCONTEXT_H
#define FKANDROID_FKWITHGLCONTEXT_H

#include "gtest/gtest.h"
#include "FkContextCompo.h"

class FkWithGLContext : public testing::Test {
    void SetUp() override;

    void TearDown() override;

    std::shared_ptr<FkContextCompo> context;
};

#endif //FKANDROID_FKWITHGLCONTEXT_H
