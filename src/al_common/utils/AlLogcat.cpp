/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlLogcat.h"

void AlLogcat::v(const string &TAG, const string fmt, ...) {
#ifdef HWDEBUG
    va_list args;
    va_start(args, fmt);
#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_VERBOSE, TAG.c_str(), fmt.c_str(), args);
#else
    printf(fmt.c_str(), args);
#endif
    va_end(args);
#endif
}

void AlLogcat::d(const string &TAG, const string fmt, ...) {
#ifdef HWDEBUG
    va_list args;
    va_start(args, fmt);
#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_DEBUG, TAG.c_str(), fmt.c_str(), args);
#else
    printf(fmt.c_str(), args);
#endif
    va_end(args);
#endif
}

void AlLogcat::i(const string &TAG, const string fmt, ...) {
#ifdef HWDEBUG
    va_list args;
    va_start(args, fmt);
#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_INFO, TAG.c_str(), fmt.c_str(), args);
#else
    printf(fmt.c_str(), args);
#endif
    va_end(args);
#endif
}

void AlLogcat::e(const string &TAG, const string fmt, ...) {
#ifdef HWDEBUG
    va_list args;
    va_start(args, fmt);
#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_ERROR, TAG.c_str(), fmt.c_str(), args);
#else
    printf(fmt.c_str(), args);
#endif
    va_end(args);
#endif
}

void AlLogcat::w(const string &TAG, const string fmt, ...) {
#ifdef HWDEBUG
    va_list args;
    va_start(args, fmt);
#ifdef ANDROID
    __android_log_vprint(ANDROID_LOG_WARN, TAG.c_str(), fmt.c_str(), args);
#else
    printf(fmt.c_str(), args);
#endif
    va_end(args);
#endif
}
