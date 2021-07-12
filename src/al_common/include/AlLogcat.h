/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALLOGCAT_H
#define HWVC_ANDROID_ALLOGCAT_H

#include "AlString.h"
#include "stdio.h"
#include <string>
#include <stdarg.h>
#include <inttypes.h>

#ifdef ANDROID

#include <android/log.h>

#endif

using namespace std;

class AlLogcat {
public:
    static void v(const string &TAG, const string fmt, ...);

    static void d(const string &TAG, const string fmt, ...);

    static void i(const string &TAG, const string fmt, ...);

    static void e(const string &TAG, const string fmt, ...);

    static void w(const string &TAG, const string fmt, ...);
};

#define AlLogFormat(fmt)                                                  \
AlString().append("%s: %d ").append(fmt).c_str()                       \

#define AlLogV(tag, fmt, args...)                                         \
AlLogcat::v(tag, AlLogFormat(fmt), __FILE_NAME__, __LINE__, ##args)        \

#define AlLogD(tag, fmt, args...)                                         \
AlLogcat::d(tag, AlLogFormat(fmt), __FILE_NAME__, __LINE__, ##args)        \

#define AlLogI(tag, fmt, args...)                                         \
AlLogcat::i(tag, AlLogFormat(fmt), __FILE_NAME__, __LINE__, ##args)        \

#define AlLogE(tag, fmt, args...)                                         \
AlLogcat::e(tag, AlLogFormat(fmt), __FILE_NAME__, __LINE__, ##args)        \

#define AlLogW(tag, fmt, args...)                                         \
AlLogcat::w(tag, AlLogFormat(fmt), __FILE_NAME__, __LINE__, ##args)        \

#if !defined(__AL_DEBUG__)
#undef AlLogD
#undef AlLogV
#define AlLogV(tag, fmt, args...)
#define AlLogD(tag, fmt, args...)
#endif

#endif //HWVC_ANDROID_ALLOGCAT_H
