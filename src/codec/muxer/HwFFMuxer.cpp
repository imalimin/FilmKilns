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
    avPacket = av_packet_alloc();
    return Hw::SUCCESS;
}

HwResult HwFFMuxer::start() {
    if (avio_open2(&pFormatCtx->pb, filePath.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) {
        release();
        Logcat::e("HWVC", "HwFFMuxer::initialize failed to open output file!");
        return Hw::FAILED;
    }
    int ret = avformat_write_header(pFormatCtx, nullptr);
    if (ret < 0) {
        release();
        Logcat::e("HWVC", "HwFFMuxer::initialize failed to write header!");
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}

void HwFFMuxer::setInt32Parameter(int32_t &param, int32_t value) {
    if (HwBundle::VALUE_NONE != value) {
        param = value;
    }
}

void HwFFMuxer::setInt64Parameter(int64_t &param, int64_t value) {
    if (HwBundle::VALUE_NONE != value) {
        param = value;
    }
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
    setInt32Parameter(stream->codecpar->profile,
                      codec->getFormat()->getInt32(HwAbsCodec::KEY_PROFILE));
    setInt32Parameter(stream->codecpar->level, codec->getFormat()->getInt32(HwAbsCodec::KEY_LEVEL));
    setInt64Parameter(stream->codecpar->bit_rate,
                      codec->getFormat()->getInt32(HwAbsCodec::KEY_BIT_RATE));
    if (1 == codec->type()) {
        stream->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
        setInt32Parameter(stream->codecpar->sample_rate,
                          codec->getFormat()->getInt32(HwAbsCodec::KEY_SAMPLE_RATE));
        setInt32Parameter(stream->codecpar->channels,
                          codec->getFormat()->getInt32(HwAbsCodec::KEY_CHANNELS));
        setInt32Parameter(stream->codecpar->frame_size,
                          codec->getFormat()->getInt32(HwAbsCodec::KEY_FRAME_SIZE));
        stream->codecpar->channel_layout = static_cast<uint64_t>(
                av_get_default_channel_layout(stream->codecpar->channels));
        stream->codecpar->format = AV_SAMPLE_FMT_FLTP;
//        stream->time_base = {1, stream->codecpar->sample_rate};
    } else if (0 == codec->type()) {
        stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
        setInt32Parameter(stream->codecpar->width,
                          codec->getFormat()->getInt32(HwAbsCodec::KEY_WIDTH));
        setInt32Parameter(stream->codecpar->height,
                          codec->getFormat()->getInt32(HwAbsCodec::KEY_HEIGHT));
        stream->codecpar->format = AV_PIX_FMT_YUV420P;
//        stream->time_base = {1, codec->getFormat()->getInt32(HwAbsCodec::KEY_FPS)};
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
                stream->codecpar->extradata_size = sps->size() + pps->size();
                int32_t offset = 0;
                uint8_t *extra = static_cast<uint8_t *>(av_mallocz(
                        stream->codecpar->extradata_size));
                memcpy(extra + offset, sps->getData(), sps->size());
                offset += sps->size();
                memcpy(extra + offset, pps->getData(), pps->size());
                stream->codecpar->extradata = extra;
                FILE *fp = fopen("/sdcard/extra.data", "wb");
                fwrite(stream->codecpar->extradata, 1, stream->codecpar->extradata_size, fp);
                fclose(fp);
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

HwResult HwFFMuxer::write(int32_t track, HwPacket *pkt) {
    if (!pkt || track > tracks.size() - 1 || !pFormatCtx) {
        return Hw::FAILED;
    }
    pkt->ref(&avPacket);
    avPacket->stream_index = tracks[track]->index;
    AVRational tb = tracks[track]->time_base;
    // pts / cq * bq
    av_packet_rescale_ts(avPacket, AV_TIME_BASE_Q, tb);
    if (tracks[track]->cur_dts >= avPacket->dts) {
        Logcat::i("HWVC", "HwFFMuxer::write will failed(%lld, %lld), try reset correctly.",
                  tracks[track]->cur_dts, avPacket->dts);
        avPacket->dts += 1;
    }
    int ret = av_interleaved_write_frame(pFormatCtx, avPacket);
    if (0 != ret) {
        Logcat::e("HWVC", "HwFFMuxer::write failed(track %d): %s", track, strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}