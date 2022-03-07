//
// Created by Alimin on 2021/8/6.
//

#include "FkWithGLContext.h"

void FkWithGLContext::SetUp() {
    context = std::make_shared<FkContextCompo>("Test");
    EXPECT_EQ(context->create(), FK_OK);
    EXPECT_EQ(context->makeCurrent(), FK_OK);
    EXPECT_NE(eglGetCurrentContext(), EGL_NO_CONTEXT);
}

void FkWithGLContext::TearDown() {
    context->destroy();
    EXPECT_EQ(eglGetCurrentContext(), EGL_NO_CONTEXT);
}

