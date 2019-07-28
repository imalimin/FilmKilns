/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/WAVRawMuxer.h"
#include "../include/HwAbsMediaFrame.h"
#include "Logcat.h"
#include "byteswap.h"

static int32_t BLEndianUint32(int32_t value) {
//    return bswap_32(value);
    return value;
}

static int16_t BLEndianUint16(int16_t value) {
//    return bswap_16(value);
    return value;
}

WAVRawMuxer *WAVRawMuxer::build(std::string filePath, HwSampleFormat format) {
    return new WAVRawMuxer(filePath, format);
}

WAVRawMuxer::WAVRawMuxer(std::string filePath, HwSampleFormat format) : Object(),
                                                                        filePath(filePath),
                                                                        format(format) {
    header = static_cast<Header *>(malloc(sizeof(Header)));
    header->riff[0] = 'R';
    header->riff[1] = 'I';
    header->riff[2] = 'F';
    header->riff[3] = 'F';
    header->riffSize = 0;
    header->riffType[0] = 'W';
    header->riffType[1] = 'A';
    header->riffType[2] = 'V';
    header->riffType[3] = 'E';
    header->subChunkFmt[0] = 'f';
    header->subChunkFmt[1] = 'm';
    header->subChunkFmt[2] = 't';
    header->subChunkFmt[3] = ' ';
    header->fmtSize = BLEndianUint32(16);
    header->format = BLEndianUint16(1);
    header->channels = BLEndianUint16(this->format.getChannels());
    header->sampleRate = BLEndianUint32(this->format.getSampleRate());
    header->byteRate = BLEndianUint32(this->format.getSampleRate() *
                                      this->format.getChannels() *
                                      HwAbsMediaFrame::getBytesPerSample(
                                              this->format.getFormat()));
    header->blockAlign = BLEndianUint16(static_cast<int16_t>(
                                                this->format.getChannels() *
                                                HwAbsMediaFrame::getBytesPerSample(
                                                        this->format.getFormat())));
    header->bitsPerSample = BLEndianUint16(static_cast<int16_t>(
                                                   HwAbsMediaFrame::getBytesPerSample(
                                                           this->format.getFormat()) * 8));
    header->subChunkData[0] = 'd';
    header->subChunkData[1] = 'a';
    header->subChunkData[2] = 't';
    header->subChunkData[3] = 'a';
    header->dataSize = 0;
    file = fopen(filePath.c_str(), "wb");
    writeHeader();
}

WAVRawMuxer::~WAVRawMuxer() {
    writeTail();
    if (file) {
        fclose(file);
        file = nullptr;
    }
    if (header) {
        free(header);
        header = nullptr;
    }
}

HwResult WAVRawMuxer::write(HwBuffer *buf) {
    if (file) {
        int count = fwrite(buf->getData(), 1, buf->size(), file);
        size += count;
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult WAVRawMuxer::writeHeader() {
    if (file && header) {
        fwrite(header, sizeof(Header), 1, file);
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult WAVRawMuxer::writeTail() {
    if (file && header) {
        header->riffSize = BLEndianUint32(size + sizeof(Header) - 4);
        header->dataSize = BLEndianUint32(size);
        fseek(file, 0, SEEK_SET);
        return writeHeader();
    }
    return Hw::FAILED;
}