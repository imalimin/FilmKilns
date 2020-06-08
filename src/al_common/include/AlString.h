/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALSTRING_H
#define HWVC_ANDROID_ALSTRING_H

#include "Object.h"
#include <string>
#include <sstream>

using namespace std;

al_class AlString al_extend Object {
public:
    static std::string valueOf(int32_t value);

    static std::string valueOf(int64_t value);

    static std::string valueOf(float value);

    static std::string valueOf(double value);

private:
    template<typename T>
    static std::string _valueOf(T value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

public:
    AlString();

    AlString(const char *str);

    AlString(const std::string &str);

    AlString(const AlString &o);

    virtual ~AlString();

    std::string &str();

    const char *c_str();

    AlString &append(const std::string &str);

    AlString &append(const char *str);

    AlString &append(int32_t val);

    AlString &append(int64_t val);

    AlString &append(float val);

    AlString &append(double val);

    size_t length();

    void clear();

    AlString &replaceAll(const char *oldStr, const char *newStr);

    virtual std::string toString() override;

private:
    std::string _str;
};


#endif //HWVC_ANDROID_ALSTRING_H
