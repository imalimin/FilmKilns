/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_HWVCFILTER_H
#define HARDWAREVIDEOCODEC_HWVCFILTER_H

#include "HwAbsFilter.h"
#include "AlPngDecoder.h"
#include <string>
#include "../tinyxml/FilterReader.h"

using namespace std;

class HwvcFilter : public HwAbsFilter {
public:
    HwvcFilter(char *path);

    virtual ~HwvcFilter();

    virtual HwProgram *createProgram() override;

    virtual void drawFirst(HwProgram *program, HwAbsTexture *src, HwAbsTexture *dest) override;

private:
    FilterReader *reader = nullptr;
    char **names = nullptr;
    char **samplers = nullptr;
    int size = 0;
    int paramSize = 0;
    float *params = nullptr;
    uint32_t *textures = nullptr;
    int32_t *textureLocations = nullptr;
    int32_t *paramLocations = nullptr;

    uint32_t loadTexture(string pngBuf);
};


#endif //HARDWAREVIDEOCODEC_HWVCFILTER_H
