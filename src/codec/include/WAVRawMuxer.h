/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_WAVRAWMUXER_H
#define HWVC_ANDROID_WAVRAWMUXER_H

#include <string>
#include "Object.h"
#include "HwSampleFormat.h"
#include "HwResult.h"
#include "HwBuffer.h"

class WAVRawMuxer : public Object {
public:
    static WAVRawMuxer *build(std::string filePath, HwSampleFormat format);

    virtual ~WAVRawMuxer();

    HwResult write(HwBuffer *buf);

private:
    HwResult writeHeader();

    HwResult writeTail();

private:
    WAVRawMuxer(std::string filePath, HwSampleFormat format);

private:
    std::string filePath;
    HwSampleFormat format;
    FILE *file = nullptr;
    uint32_t size = 0;

private:
    struct Header {
        char riff[4];
        int32_t riffSize = 0;
        char riffType[4];
        char subChunkFmt[4];
        int32_t fmtSize = 0;
        int16_t format = 0;
        int16_t channels = 0;
        int32_t sampleRate = 0;
        int32_t byteRate = 0;
        int16_t blockAlign = 0;
        int16_t bitsPerSample = 0;
        char subChunkData[4];
        int32_t dataSize = 0;
    };
    Header *header = nullptr;
};


#endif //HWVC_ANDROID_WAVRAWMUXER_H
