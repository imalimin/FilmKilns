/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_ANDROID_MYPRINTER_H
#define FK_ANDROID_MYPRINTER_H

#include "gtest/gtest.h"

#define FK_SETUP_PRINTER \
MyPrinter printer; \
::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners(); \
listeners.Append(&printer); \


class MyPrinter : public testing::EmptyTestEventListener {
    void OnTestSuiteStart(const ::testing::TestSuite &test_suite) override;

    void OnTestSuiteEnd(const ::testing::TestSuite &test_suite) override;

    virtual void OnTestStart(const ::testing::TestInfo &test_info) override;

    virtual void OnTestEnd(const ::testing::TestInfo &test_info) override;

    virtual void OnTestPartResult(const ::testing::TestPartResult &test_part_result) override;
};


#endif //FK_ANDROID_MYPRINTER_H
