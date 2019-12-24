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

class AlBitmapFactory : public Object {
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

private:
    static int32_t _guessFormat(std::string path);

    static int32_t _guessFormat(uint8_t *buf);

private:
    static constexpr int32_t FORMAT_UNSUPPORTED = -1;
    static constexpr int32_t FORMAT_JPEG = 0;
    static constexpr int32_t FORMAT_PNG = 1;
    static constexpr int32_t FORMAT_WEBP = 2;

};


#endif //HARDWAREVIDEOCODEC_HWBITMAPFACTORY_H
