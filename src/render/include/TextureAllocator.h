//
// Created by mingyi.li on 2018/12/27.
//

#ifndef HARDWAREVIDEOCODEC_TEXTURECENTR_H
#define HARDWAREVIDEOCODEC_TEXTURECENTR_H

#include "Object.h"
#include <vector>
#include "HwAbsTexture.h"
#include "AlBuffer.h"

using namespace std;

class TextureAllocator : public Object {
public:
    TextureAllocator();

    virtual ~TextureAllocator();

    HwAbsTexture *alloc();

    HwAbsTexture *alloc(AlTexDescription &desc, AlBuffer *buf = nullptr);

    void recycle(HwAbsTexture **tex);

private:
    TextureAllocator(const TextureAllocator &o) : Object() {};

private:
    vector<HwAbsTexture *> textures;

    void clear();
};


#endif //HARDWAREVIDEOCODEC_TEXTURECENTR_H
