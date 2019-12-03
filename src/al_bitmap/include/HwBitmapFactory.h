/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HARDWAREVIDEOCODEC_HWBITMAPFACTORY_H
#define HARDWAREVIDEOCODEC_HWBITMAPFACTORY_H

#include "Object.h"
#include "HwBitmap.h"
#include "AlBuffer.h"
#include "string"

class HwBitmapFactory : public Object {
public:
    static HwBitmap *decodeFile(std::string file);

    /**
     * @param  file          文件路径
     * @param  recycleBitmap 复用bitmap
     * @return HwBitmap
     */
    static HwBitmap *decodeFile(std::string file, HwBitmap *recycleBitmap);

    static HwBitmap *decodeBuffer(AlBuffer *srcBuf);

    static HwResult save(int32_t w, int32_t h, AlBuffer *buf, std::string path);

    static HwResult save(int32_t w, int32_t h, AlBuffer *buf,
                         int32_t quality, std::string path);

};


#endif //HARDWAREVIDEOCODEC_HWBITMAPFACTORY_H
