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
    static void i(const string &TAG, const string fmt, ...);

    static void e(const string &TAG, const string fmt, ...);

    static void w(const string &TAG, const string fmt, ...);
};

#define AlLogFormat(fmt)                                                  \
AlString().append("%s(%d): ").append(fmt).c_str()                       \

#define AlLogI(tag, fmt, args...)                                         \
AlLogcat::i(tag, AlLogFormat(fmt), __FUNCTION__, __LINE__, ##args)        \

#define AlLogE(tag, fmt, args...)                                         \
AlLogcat::e(tag, AlLogFormat(fmt), __FUNCTION__, __LINE__, ##args)        \

#define AlLogW(tag, fmt, args...)                                         \
AlLogcat::w(tag, AlLogFormat(fmt), __FUNCTION__, __LINE__, ##args)        \


#endif //HWVC_ANDROID_ALLOGCAT_H
