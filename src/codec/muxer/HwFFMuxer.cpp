/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFFMuxer.h"
#include "Logcat.h"

HwFFMuxer::HwFFMuxer() : HwAbsMuxer() {
}

HwFFMuxer::~HwFFMuxer() {
    release();
}

void HwFFMuxer::release() {
    if (pFormatCtx) {
        av_write_trailer(pFormatCtx);
    }
    tracks.clear();
    if (pFormatCtx) {
        if (!(pFormatCtx->flags & AVFMT_NOFILE)) {
            avio_closep(&pFormatCtx->pb);
        }
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
}

HwResult HwFFMuxer::configure(string filePath, string type) {
    HwAbsMuxer::configure(filePath, type);
    av_register_all();
    int ret = avformat_alloc_output_context2(&pFormatCtx, NULL, type.c_str(), filePath.c_str());
    if (ret < 0 || !pFormatCtx) {
        Logcat::e("HWVC", "HwFFMuxer::configure failed %s", strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    av_dict_set(&pFormatCtx->metadata, "comment", "hwvc", 0);
    return Hw::SUCCESS;
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

int32_t HwFFMuxer::addTrack(HwAbsCodec *codec) {
    if (!codec || !codec->getFormat()) {
        return TRACK_NONE;
    }
    AVStream *stream = avformat_new_stream(pFormatCtx, nullptr);
    if (!stream) {
        return TRACK_NONE;
    }

    // See avcodec_parameters_from_context
    stream->codecpar->codec_id = static_cast<AVCodecID>(codec->getCodecId());
    stream->codecpar->profile = codec->getFormat()->getInt32(HwAbsCodec::KEY_PROFILE);
    stream->codecpar->level = codec->getFormat()->getInt32(HwAbsCodec::KEY_LEVEL);
    stream->codecpar->bit_rate = codec->getFormat()->getInt64(HwAbsCodec::KEY_BIT_RATE);
    if (1 == codec->type()) {
        stream->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
        stream->codecpar->sample_rate = codec->getFormat()->getInt32(HwAbsCodec::KEY_SAMPLE_RATE);
        stream->codecpar->channels = codec->getFormat()->getInt32(HwAbsCodec::KEY_CHANNELS);
        stream->codecpar->channel_layout = static_cast<uint64_t>(
                av_get_default_channel_layout(stream->codecpar->channels));
        stream->codecpar->format = AV_SAMPLE_FMT_FLTP;
        stream->codecpar->frame_size = codec->getFormat()->getInt32(HwAbsCodec::KEY_FRAME_SIZE);
        stream->time_base = {1, stream->codecpar->sample_rate};
    } else if (0 == codec->type()) {
        stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
        stream->codecpar->width = codec->getFormat()->getInt32(HwAbsCodec::KEY_WIDTH);
        stream->codecpar->height = codec->getFormat()->getInt32(HwAbsCodec::KEY_HEIGHT);
        stream->codecpar->format = AV_PIX_FMT_YUV420P;
        stream->time_base = {1, codec->getFormat()->getInt32(HwAbsCodec::KEY_FPS)};
    }
    copyExtraData(stream, codec);
    tracks.push_back(stream);
    return tracks.size() - 1;
}

bool HwFFMuxer::copyExtraData(AVStream *stream, HwAbsCodec *codec) {
    switch (stream->codecpar->codec_id) {
        case AV_CODEC_ID_H264: {
            HwBuffer *sps = codec->getExtraBuffer("csd-0");
            HwBuffer *pps = codec->getExtraBuffer("csd-1");
            if (sps && pps) {
                stream->codecpar->extradata_size = sps->size() + pps->size() + 8;
                uint8_t flag[4] = {0, 0, 0, 1};
                int32_t offset = 0;
                uint8_t *extra = static_cast<uint8_t *>(av_mallocz(
                        stream->codecpar->extradata_size));
                memcpy(extra + offset, &flag, 4);
                offset += 4;
                memcpy(extra + offset, sps->getData(), sps->size());
                offset += sps->size();
                memcpy(extra + offset, &flag, 4);
                offset += 4;
                memcpy(extra + offset, pps->getData(), pps->size());
                stream->codecpar->extradata = extra;
            }
            break;
        }
        case AV_CODEC_ID_AAC_LATM:
        case AV_CODEC_ID_AAC: {
            HwBuffer *esds = codec->getExtraBuffer("csd-0");
            if (esds) {
                stream->codecpar->extradata_size = esds->size();
                uint8_t *extra = static_cast<uint8_t *>(av_mallocz(esds->size()));
                memcpy(extra, esds->getData(), esds->size());
                stream->codecpar->extradata = extra;
            }
            break;
        }
        default:
            break;
    }
    return true;
}

HwResult HwFFMuxer::write(int32_t track, void *packet) {
    if (!packet || track > tracks.size() - 1 || !pFormatCtx) {
        return Hw::FAILED;
    }
    AVPacket *pkt = static_cast<AVPacket *>(packet);
    pkt->pts = av_rescale_q_rnd(pkt->pts,
                                AV_TIME_BASE_Q,
                                tracks[track]->time_base,
                                AV_ROUND_NEAR_INF);
    pkt->dts = av_rescale_q_rnd(pkt->dts,
                                AV_TIME_BASE_Q,
                                tracks[track]->time_base,
                                AV_ROUND_NEAR_INF);
    pkt->duration = av_rescale_q_rnd(pkt->duration,
                                     AV_TIME_BASE_Q,
                                     tracks[track]->time_base,
                                     AV_ROUND_NEAR_INF);
    pkt->stream_index = tracks[track]->index;
    int ret = av_write_frame(pFormatCtx, pkt);
    Logcat::i("HWVC", "HwFFMuxer::write %d, %lld, %lld", track, pkt->pts, pkt->duration);
    if (0 != ret) {
        Logcat::i("HWVC", "HwFFMuxer::write failed: %s", strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}