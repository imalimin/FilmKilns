/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFFMuxer.h"

HwFFMuxer::HwFFMuxer(string filePath, string type) : HwAbsMuxer(filePath, type) {
    av_register_all();
    avformat_alloc_output_context2(&pFormatCtx, NULL, type.c_str(), filePath.c_str());
    av_dict_set(&pFormatCtx->metadata, "comment", "hwvc", 0);
}

HwFFMuxer::~HwFFMuxer() {
    tracks.clear();
    if (pFormatCtx) {
        if (!(pFormatCtx->flags & AVFMT_NOFILE)) {
            avio_closep(&pFormatCtx->pb);
        }
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
}

int32_t HwFFMuxer::addAudioTrack() {
    AVStream *stream = avformat_new_stream(pFormatCtx, nullptr);
    tracks.emplace_back(stream);
    return tracks.size() - 1;
}

int32_t HwFFMuxer::addVideoTrack() {
    AVStream *stream = avformat_new_stream(pFormatCtx, nullptr);
    tracks.emplace_back(stream);
    return tracks.size() - 1;
}

bool HwFFMuxer::write(int32_t track) {
    return true;
}