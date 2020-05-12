//
// Created by limin on 2019/6/29.
//

#include "StringUtils.h"
#include <math.h>

std::string StringUtils::trimLeft(const std::string &str, const std::string &token) {
    std::string t = str;
    t.erase(0, t.find_first_not_of(token));
    return t;
}

std::string StringUtils::trimRight(const std::string &str, const std::string &token) {
    std::string t = str;
    t.erase(t.find_last_not_of(token) + 1);
    return t;
}

std::string StringUtils::trim(const std::string &str, const std::string &token) {
    std::string t = str;
    t.erase(0, t.find_first_not_of(token));
    t.erase(t.find_last_not_of(token) + 1);
    return t;
}

std::string StringUtils::toLower(const std::string &str) {
    std::string t = str;
    std::transform(t.begin(), t.end(), t.begin(), tolower);
    return t;
}

std::string StringUtils::toUpper(const std::string &str) {
    std::string t = str;
    std::transform(t.begin(), t.end(), t.begin(), toupper);
    return t;
}

bool StringUtils::startsWith(const std::string &str, const std::string &substr) {
    return str.find(substr) == 0;
}

bool StringUtils::endsWith(const std::string &str, const std::string &substr) {
    return str.rfind(substr) == (str.length() - substr.length());
}

bool StringUtils::equalsIgnoreCase(const std::string &str1, const std::string &str2) {
    return toLower(str1) == toLower(str2);
}

std::vector<std::string> StringUtils::split(const std::string &str, const std::string &delimiter) {
    char *save = nullptr;
    char *token = strtok_r(const_cast<char *>(str.c_str()), delimiter.c_str(), &save);
    std::vector<std::string> result;
    while (token != nullptr) {
        result.emplace_back(token);
        token = strtok_r(nullptr, delimiter.c_str(), &save);
    }
    return result;
}

std::string StringUtils::valueOf(int32_t value) {
    return valueOf<int32_t>(value);
}

std::string StringUtils::valueOf(int64_t value) {
    return valueOf<int64_t>(value);
}

std::string StringUtils::valueOf(float value) {
    return valueOf<float>(value);
}

std::string StringUtils::valueOf(double value) {
    return valueOf<double>(value);
}

bool StringUtils::isEmpty(std::string *str) {
    return nullptr == str || "" == *str || str->empty();
}

int32_t StringUtils::toInt(const char *str) {
    if (nullptr == str) {
        return INT32_MIN;
    }
    return atoi(str);
}

int64_t StringUtils::toLong(const char *str) {
    if (nullptr == str) {
        return INT64_MIN;
    }
    return atoll(str);
}

float StringUtils::toFloat(const char *str) {
    if (nullptr == str) {
        return MAXFLOAT;
    }
    return static_cast<float>(atof(str));
}

double StringUtils::toDouble(const char *str) {
    if (nullptr == str) {
        return MAXFLOAT;
    }
    return atof(str);
}
