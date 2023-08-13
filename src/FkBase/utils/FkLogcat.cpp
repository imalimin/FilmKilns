/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-26 22:10:11
*/

#include "FkLogcat.h"
#include "jni.h"

#ifdef ANDROID

#include <android/log.h>

#endif

void FkLogcat::v(const std::string &TAG, const std::string fmt, ...) {
#if defined(__FK_DEBUG__)
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

void FkLogcat::d(const std::string &TAG, const std::string fmt, ...) {
#if defined(__FK_DEBUG__)
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

void FkLogcat::i(const std::string &TAG, const std::string fmt, ...) {
#if defined(__FK_DEBUG__) || defined(__ENABLE_FORCE_OUTPUT_INFO__)
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

void FkLogcat::e(const std::string &TAG, const std::string fmt, ...) {
#if defined(__FK_DEBUG__) || defined(__ENABLE_FORCE_OUTPUT_INFO__)
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

void FkLogcat::w(const std::string &TAG, const std::string fmt, ...) {
#if defined(__FK_DEBUG__) || defined(__ENABLE_FORCE_OUTPUT_INFO__)
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