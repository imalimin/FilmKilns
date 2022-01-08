/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "MyPrinter.h"
#include "FkDefinition.h"
#include "FkTestResultHolder.h"

#define TAG "FkTest"

void MyPrinter::OnTestSuiteStart(const ::testing::TestSuite &test_suite) {
    AlLogcat::i(TAG, "[ START  ] Total test count %d", test_suite.total_test_count());
    AlLogcat::i(TAG, "[ ====== ]");
}

void MyPrinter::OnTestSuiteEnd(const ::testing::TestSuite &test_suite) {
    AlLogcat::i(TAG, "[ ====== ]");
    AlLogcat::i(TAG, "[ FINISH ] Passed test count %d/%d", test_suite.successful_test_count(),
                test_suite.total_test_count());

}

void MyPrinter::OnTestStart(const ::testing::TestInfo &test_info) {
    AlLogcat::i(TAG, "[  RUN   ] %s: %s", test_info.test_case_name(), test_info.name());
}

void MyPrinter::OnTestEnd(const ::testing::TestInfo &test_info) {
    auto *result = test_info.result();
    auto passed = result && result->Passed();
    std::string fmt = "%s %s Line: %d";
    char msg[512];
    if (passed) {
        sprintf(msg, fmt.c_str(),
                "[ PASSED ]",
                test_info.test_case_name(),
                test_info.line());
    } else {
        sprintf(msg, fmt.c_str(),
                "[ FAILED ]",
                test_info.test_case_name(),
                test_info.line());
    }
    AlLogcat::i(TAG, "%s", msg);
    FkTestResultHolder::getInstance()->push(msg, passed);
}

void MyPrinter::OnTestPartResult(const ::testing::TestPartResult &test_part_result) {
}