/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HARDWAREVIDEOCODEC_TEXTURECENTR_H
#define HARDWAREVIDEOCODEC_TEXTURECENTR_H

#include "Object.h"
#include <vector>
#include "HwAbsTexture.h"
#include "AlBuffer.h"

using namespace std;

class AlTexAllocator : public Object {
public:
    AlTexAllocator();

    virtual ~AlTexAllocator();

    HwAbsTexture *alloc();

    HwAbsTexture *alloc(AlTexDescription &desc, AlBuffer *buf = nullptr);

    bool recycle(HwAbsTexture **tex);

    size_t size();

    bool empty();

private:
    AlTexAllocator(const AlTexAllocator &o) : Object() {};

private:
    vector<HwAbsTexture *> textures;

    void clear();
};


#endif //HARDWAREVIDEOCODEC_TEXTURECENTR_H
