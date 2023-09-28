/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkTimeUtils.h"
#include <chrono>

int64_t FkTimeUtils::getCurrentTimeUS() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return static_cast<long long int>(tv.tv_sec * 1000000.0 + tv.tv_usec);
}

std::string FkTimeUtils::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm *now_tm = std::localtime(&t);

    char buffer[21];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);
    return buffer;
}