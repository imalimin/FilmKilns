/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2022-05-14 11:04:36
*/

#include "FkWorkspace.h"
#include "FkString.h"
#include "FkFileUtils.h"

FK_IMPL_CLASS_TYPE(FkWorkspace, FkObject)

FkWorkspace::FkWorkspace(std::string _dir) : FkObject(), dir(_dir) {
    FkFileUtils::mkdirs(dir);
}

FkWorkspace::FkWorkspace(const FkWorkspace &o) : FkObject(o) {

}

FkWorkspace::~FkWorkspace() {

}

void FkWorkspace::clear() {
//    auto ret = FkFileUtils::remove(dir);
//    FkAssert(ret == true,);
}

FkResult FkWorkspace::includeAsSource(std::string src, std::string cacheName) {
    std::string dst = FkString(dir)
            .append("/")
            .append(cacheName)
            .toString();
    if (src == dst) {
        return FK_OK;
    }
    return FkFileUtils::copy(src, dst);
}

std::string FkWorkspace::getSourcePath(std::string cacheName) {
    std::string dst = FkString(dir)
            .append("/")
            .append(cacheName)
            .toString();
    return dst;
}
