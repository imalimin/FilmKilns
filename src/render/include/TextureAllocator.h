//
// Created by mingyi.li on 2018/12/27.
//

#ifndef HARDWAREVIDEOCODEC_TEXTURECENTR_H
#define HARDWAREVIDEOCODEC_TEXTURECENTR_H

#include "Object.h"
#include <vector>
#include "HwAbsTexture.h"

using namespace std;

class TextureAllocator : public Object {
public:
    TextureAllocator();

    virtual ~TextureAllocator();

    HwAbsTexture *alloc();

    HwAbsTexture *alloc(uint8_t *rgba, int width, int height, uint32_t fmt);

    void recycle(HwAbsTexture **tex);

private:
    vector<HwAbsTexture *> textures;

    void clear();
};


#endif //HARDWAREVIDEOCODEC_TEXTURECENTR_H
