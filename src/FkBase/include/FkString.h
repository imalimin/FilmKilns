/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKSTRING_H
#define FK_BASE_FKSTRING_H

#include "FkObject.h"
#include <vector>
#include <sstream>

class FkString;

FK_SUPER_CLASS(FkStringBuilder, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkStringBuilder)

public:
    FkStringBuilder(const FkString &str);

    ~FkStringBuilder() = default;
};

FK_SUPER_CLASS(FkString, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkString)

public:
    static std::string valueOf(const std::string &value);

    static std::string valueOf(const FkString &value);

    template<typename T>
    static std::string valueOf(const T &value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    static std::vector<std::string> split(const std::string &str,const std::string &pattern);

public:
    FkString();

    FkString(const char *str);

    FkString(const std::string &str);

    FkString(const FkString &o);

    virtual ~FkString();

    std::string &str();

    const char *c_str();

    const char *c_str() const;

    FkString &begin();

    FkString &end();

    size_t length();

    void clear();

    FkString &replaceAll(const char *oldStr, const char *newStr);

    virtual std::string toString() override;

    bool startWith(std::string str);

    template<typename T>
    FkString &append(const T &val) {
        if (appendBegin) {
            _str = valueOf(val) + _str;
        } else {
            _str.append(valueOf(val));
        }
        return *this;
    }

    template<typename T>
    FkString &operator<<(const T &val) {
        append(val);
        return *this;
    }

private:
    std::string _str;
    bool appendBegin = false;
};

#endif //FK_BASE_FKSTRING_H