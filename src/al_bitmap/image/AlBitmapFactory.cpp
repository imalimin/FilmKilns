/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlBitmapFactory.h"
#include "AlJpegDecoder.h"
#include "AlPngDecoder.h"
#include "Logcat.h"
#include "AlJpegEncoder.h"
#include "AlPngEncoder.h"
#include "AlWebPDecoder.h"
#include "AlWebPEncoder.h"
#include "AlBMPEncoder.h"
#include "StringUtils.h"
#include "AlObjectGuard.h"

#define TAG "AlBitmapFactory"

AlBitmap *AlBitmapFactory::decodeFile(std::string file) {
    int32_t format = _guessFormat(file);
    AlBitmapInfo info;
    AlBuffer *buf = nullptr;
    AlAbsDecoder *decoder = nullptr;
    AlObjectGuard guard((Object **) &decoder);
    switch (format) {
        case FORMAT_JPEG:
            decoder = new AlJpegDecoder(file);
            break;
        case FORMAT_PNG:
            decoder = new AlPngDecoder(file);
            break;
        case FORMAT_WEBP:
            decoder = new AlWebPDecoder(file);
            break;
        default:
            decoder = nullptr;
    }
    if (nullptr == decoder) {
        Logcat::i(TAG, "%s(%d): failed(%s)", __FUNCTION__, __LINE__, file.c_str());
        return nullptr;
    }
    info = decoder->getInfo();
    if (info.isNull()) {
        Logcat::i(TAG, "%s(%d): failed(%s)", __FUNCTION__, __LINE__, file.c_str());
        return nullptr;
    }
    HwResult ret = decoder->process(&buf, &info);
    if (Hw::SUCCESS != ret) {
        Logcat::i(TAG, "%s(%d): failed(%s)", __FUNCTION__, __LINE__, file.c_str());
        return nullptr;
    }
    AlBitmap *bitmap = AlBitmap::create(info);
    memcpy(bitmap->getPixels(), buf->data(), static_cast<size_t>(bitmap->getByteSize()));
    bitmap->dump();
    delete buf;
    return bitmap;
}

AlBitmap *AlBitmapFactory::decodeFile(std::string file, AlBitmap *recycleBitmap) {
    AlLogE(TAG, "Not support function.");
    return nullptr;
}

AlBitmap *AlBitmapFactory::decodeBuffer(AlBuffer *srcBuf) {
    int32_t format = _guessFormat(srcBuf->data());
    AlBitmapInfo info;
    AlBuffer *buf = nullptr;
    AlAbsDecoder *decoder = nullptr;
    AlObjectGuard guard((Object **) &decoder);
    switch (format) {
        case FORMAT_PNG:
            decoder = new AlPngDecoder(srcBuf);
            break;
        default:
            decoder = nullptr;
    }
    if (nullptr == decoder) {
        Logcat::i(TAG, "%s(%d): failed", __FUNCTION__, __LINE__);
        return nullptr;
    }
    info = decoder->getInfo();
    if (info.isNull()) {
        Logcat::i(TAG, "%s(%d): failed", __FUNCTION__, __LINE__);
        return nullptr;
    }
    HwResult ret = decoder->process(&buf, &info);
    if (Hw::SUCCESS != ret) {
        Logcat::i(TAG, "%s(%d): failed", __FUNCTION__, __LINE__);
        return nullptr;
    }
    AlBitmap *bitmap = AlBitmap::create(info);
    memcpy(bitmap->getPixels(), buf->data(), static_cast<size_t>(bitmap->getByteSize()));
    bitmap->dump();
    delete buf;
    return bitmap;
}

HwResult AlBitmapFactory::save(int32_t w, int32_t h, AlBuffer *buf, std::string path) {
    return save(w, h, buf, 80, path);
}

HwResult AlBitmapFactory::save(int32_t w, int32_t h, AlBuffer *buf,
                               int32_t quality, std::string path) {
    if (path.empty()) return Hw::FAILED;
    AlBitmapInfo info;
    info.width = w;
    info.height = h;
    info.depth = 4;
    info.colorSpace = AlColorSpace::RGBA;
    AlAbsEncoder *encoder = nullptr;
    switch (_getFileSuffix(path)) {
        case FORMAT_PNG: {
            encoder = new AlPngEncoder(path);
            break;
        }
        case FORMAT_JPEG: {
            encoder = new AlJpegEncoder(path);
            break;
        }
        case FORMAT_BMP: {
            encoder = new AlBMPEncoder();
            break;
        }
        case FORMAT_WEBP: {
            encoder = new AlWebPEncoder(path);
            break;
        }
    }
    if (nullptr == encoder) {
        return Hw::FAILED;
    }
    HwResult ret = encoder->process(buf, &info, path);
    delete encoder;
    return ret;
}

static WEBP_INLINE uint32_t GetBE32(const uint8_t buf[]) {
    return ((uint32_t) buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}

int32_t AlBitmapFactory::_guessFormat(std::string path) {
    FILE *in = fopen(path.c_str(), "rb");
    if (nullptr == in) {
        Logcat::i(TAG, "%s(%d): failed", __FUNCTION__, __LINE__);
        return FORMAT_UNSUPPORTED;
    }
    uint8_t *data = new uint8_t[16];
    fread(data, 12, 1, in);
    int32_t format = _guessFormat(data);
    fclose(in);
    delete[] data;
    return format;
}

int32_t AlBitmapFactory::_guessFormat(uint8_t *buf) {
    int32_t format = FORMAT_UNSUPPORTED;
    const uint32_t magic1 = GetBE32(buf + 0);
    const uint32_t magic2 = GetBE32(buf + 8);
    if (magic1 == 0x89504E47U) {
        format = FORMAT_PNG;
    } else if (magic1 >= 0xFFD8FF00U && magic1 <= 0xFFD8FFFFU) {
        format = FORMAT_JPEG;
    } else if (magic1 == 0x52494646 && magic2 == 0x57454250) {
        format = FORMAT_WEBP;
    }
//    else if (((magic1 >> 24) & 0xff) == 'P') {
//        const int type = (magic1 >> 16) & 0xff;
//        // we only support 'P5 -> P7' for now.
//        if (type >= '5' && type <= '7') format = WEBP_PNM_FORMAT;
//    } else if (magic1 == 0x49492A00 || magic1 == 0x4D4D002A) {
//        format = WEBP_TIFF_FORMAT;
//    }
    return format;
}

int32_t AlBitmapFactory::_getFileSuffix(std::string path) {
    std::string suffix = path.substr(path.find_last_of('.') + 1);
    if (StringUtils::equalsIgnoreCase("png", suffix)) {
        return FORMAT_PNG;
    }
    if (StringUtils::equalsIgnoreCase("webp", suffix)) {
        return FORMAT_WEBP;
    }
    if (StringUtils::equalsIgnoreCase("bmp", suffix)) {
        return FORMAT_BMP;
    }
    return FORMAT_JPEG;
}
