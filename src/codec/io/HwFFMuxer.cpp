/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFFMuxer.h"
#include "Logcat.h"

HwFFMuxer::HwFFMuxer(string filePath, string type) : HwAbsMuxer(filePath, type) {
    av_register_all();
    avformat_alloc_output_context2(&pFormatCtx, NULL, type.c_str(), filePath.c_str());
    av_dict_set(&pFormatCtx->metadata, "comment", "hwvc", 0);
}

HwFFMuxer::~HwFFMuxer() {
    release();
}

void HwFFMuxer::release() {
    tracks.clear();
    if (pFormatCtx) {
        if (!(pFormatCtx->flags & AVFMT_NOFILE)) {
            avio_closep(&pFormatCtx->pb);
        }
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
}

HwResult HwFFMuxer::start() {
    if (avio_open2(&pFormatCtx->pb, filePath.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) {
        release();
        Logcat::e("HWVC", "HwFFMuxer::initialize failed to open output file!");
        return Hw::FAILED;
    }
    avformat_write_header(pFormatCtx, nullptr);
    return Hw::SUCCESS;
}

int32_t HwFFMuxer::addAudioTrack(int32_t sampleRate) {
    AVStream *stream = avformat_new_stream(pFormatCtx, nullptr);
    stream->time_base = {1, sampleRate};
    tracks.emplace_back(stream);
    return tracks.size() - 1;
}

int32_t HwFFMuxer::addVideoTrack(int32_t fps) {
    AVStream *stream = avformat_new_stream(pFormatCtx, nullptr);
    stream->time_base = {1, fps};
    tracks.emplace_back(stream);
    return tracks.size() - 1;
}

bool HwFFMuxer::write(int32_t track) {
    return true;
}