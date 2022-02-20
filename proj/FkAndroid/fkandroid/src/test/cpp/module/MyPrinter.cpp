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
    if (result == nullptr) {
        FkTestResultHolder::getInstance()->push("[ FAILED ] result is NULL.", false);
        return;
    }
    for (int i = 0; i < test_info.result()->total_part_count(); ++i) {
        auto ret = test_info.result()->GetTestPartResult(i);
        std::ostringstream caseNameOss;
        caseNameOss << test_info.test_case_name() << "-" << test_info.name();
        std::ostringstream oss;
        if (ret.passed()) {
            oss << "[ PASSED ] " << caseNameOss.str();
        } else {
            oss << "[ FAILED ] " << caseNameOss.str() << " ";
            oss << ret.file_name() << ": " << ret.line_number() << "\n";
            oss << ret.message();
        }
        FkTestResultHolder::getInstance()->push(oss.str(), ret.passed());
    }
}

void MyPrinter::OnTestPartResult(const ::testing::TestPartResult &test_part_result) {
}