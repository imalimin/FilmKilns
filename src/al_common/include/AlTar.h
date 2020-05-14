/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALTAR_H
#define HWVC_ANDROID_ALTAR_H

#include "Object.h"
#include "AlBuffer.h"

al_class AlTar al_extend Object {
public:
    al_class AlArchive al_extend Object {
    public:
        AlArchive();

        ~AlArchive();

    private:
        AlArchive(const AlArchive &o) : Object() {};
    public:
        AlBuffer *block = nullptr;
    };

public:
    virtual ~AlTar();

    static AlTar *unpack(std::string tar, std::string destDir);

private:
    AlTar();

    AlTar(const AlTar &o) : Object() {};
};


#endif //HWVC_ANDROID_ALTAR_H
