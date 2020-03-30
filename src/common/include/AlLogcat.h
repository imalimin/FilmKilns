/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALLOGCAT_H
#define HWVC_ANDROID_ALLOGCAT_H

#ifdef ANDROID

#include <android/log.h>

#endif

#include "stdio.h"
#include <string>
#include <stdarg.h>

using namespace std;

class AlLogcat {
public:
    static void i(const string &TAG, const string fmt, ...);

    static void e(const string &TAG, const string fmt, ...);

    static void w(const string &TAG, const string fmt, ...);
};

#define AlLogFormat(fmt_name, fmt)                                        \
std::string fmt_name;                                                     \
fmt_name.append("%s(%d): ");                                              \
fmt_name.append(fmt);                                                     \

#define AlLogI(tag, fmt, args...)                                         \
AlLogFormat(format, fmt)                                                  \
AlLogcat::i(tag, format.c_str(), __FUNCTION__, __LINE__, ##args)          \

#define AlLogE(tag, fmt, args...)                                         \
AlLogFormat(format, fmt)                                                  \
AlLogcat::e(tag, format.c_str(), __FUNCTION__, __LINE__, ##args)          \

#define AlLogW(tag, fmt, args...)                                         \
AlLogFormat(format, fmt)                                                  \
AlLogcat::w(tag, format.c_str(), __FUNCTION__, __LINE__, ##args)          \


#endif //HWVC_ANDROID_ALLOGCAT_H
