/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-11 20:26:34
*/

#ifndef FK_BASE_FKFILEUTILS_H
#define FK_BASE_FKFILEUTILS_H

#include "FkObject.h"

class FkFileUtils {
public:
    FkFileUtils() = delete;

    FkFileUtils(const FkFileUtils &o) = delete;

    virtual ~FkFileUtils() {};

public:
    static FkResult copy(std::string src, std::string &dst);

    static FkResult mkdirs(std::string dir);

    static bool exist(std::string &file);

    static std::string name(std::string &file);

    static std::string suffix(std::string &file);

    static std::string parent(std::string &file);

    static bool remove(std::string &file);

    static long size(std::string &file);

    static void trim(std::string &path);

    static bool startWith(const std::string &path, const std::string &temp);
};

#endif //FK_BASE_FKFILEUTILS_H