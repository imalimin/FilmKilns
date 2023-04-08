/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKSTRING_H
#define FK_BASE_FKSTRING_H

#include "FkObject.h"
#include <sstream>

FK_SUPER_CLASS(FkString, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkString)

public:
    static std::string valueOf(int32_t value);

    static std::string valueOf(int64_t value);

    static std::string valueOf(float value);

    static std::string valueOf(double value);

    static std::string valueOf(char value);

    template<typename T>
    static std::string valueOf(T value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

public:
    FkString();

    FkString(const char *str);

    FkString(const std::string &str);

    FkString(const FkString &o);

    virtual ~FkString();

    std::string &str();

    const char *c_str();

    FkString &append(const std::string &str);

    FkString &append(const char *str);

    FkString &append(int32_t val);

    FkString &append(int64_t val);

    FkString &append(float val);

    FkString &append(double val);

    FkString &append(char val);

    size_t length();

    void clear();

    FkString &replaceAll(const char *oldStr, const char *newStr);

    virtual std::string toString() override;

    bool startWith(std::string str);

private:
    std::string _str;
};

#endif //FK_BASE_FKSTRING_H