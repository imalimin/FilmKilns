/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwFFMuxer.h"
#include "AlFFUtils.h"
#include "Logcat.h"

#define TAG "HwFFMuxer"

HwFFMuxer::HwFFMuxer() : HwAbsMuxer() {
}

HwFFMuxer::~HwFFMuxer() {
    release();
}

void HwFFMuxer::release() {
    if (pFormatCtx) {
        if (state >= kState::CONFIGURED && 0 == av_write_trailer(pFormatCtx)) {
            AlLogI(TAG, "close file success.");
        } else {
            AlLogE(TAG, "close file failed!");
        }
        if (!(pFormatCtx->flags & AVFMT_NOFILE)) {
            if (0 != avio_closep(&pFormatCtx->pb)) {
                AlLogE(TAG, "close file failed!");
            }
        }
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
    tracks.clear();
    state = kState::IDL;
    AlLogI(TAG, "enter state IDL");
}

HwResult HwFFMuxer::configure(string filePath, string type) {
    HwAbsMuxer::configure(filePath, type);
    AlFFUtils::init();
    int ret = avformat_alloc_output_context2(&pFormatCtx, NULL, type.c_str(), filePath.c_str());
    if (ret < 0 || !pFormatCtx) {
        AlLogE(TAG, "failed %s", strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    av_dict_set(&pFormatCtx->metadata, "comment", "hwvc", 0);
    avPacket = av_packet_alloc();
    return Hw::SUCCESS;
}

HwResult HwFFMuxer::start() {
    mTrackConfigured.resize(tracks.size());
    for (int i = 0; i < mTrackConfigured.size(); ++i) {
        mTrackConfigured[i] = false;
    }
    state = kState::CONFIGURED;
    AlLogI(TAG, "enter state CONFIGURED");
    return Hw::SUCCESS;
}

void HwFFMuxer::setInt32Parameter(int32_t &param, int32_t value) {
    if (INT32_MIN != value) {
        param = value;
    }
}

void HwFFMuxer::setInt64Parameter(int64_t &param, int64_t value) {
    if (INT64_MIN != value) {
        param = value;
    }
}

int32_t HwFFMuxer::addTrack(AlBundle &format) {
    AlLogI(TAG, format.toString());
    AVStream *stream = avformat_new_stream(pFormatCtx, nullptr);
    if (!stream) {
        return TRACK_NONE;
    }

    // See avcodec_parameters_from_context
    stream->codecpar->codec_id = static_cast<AVCodecID>(format.get(AlCodec::KEY_CODEC_ID,
                                                                   INT32_MIN));
    setInt32Parameter(stream->codecpar->profile,
                      format.get(AlCodec::KEY_PROFILE, INT32_MIN));
    setInt32Parameter(stream->codecpar->level,
                      format.get(AlCodec::KEY_LEVEL, INT32_MIN));
    setInt64Parameter(stream->codecpar->bit_rate,
                      format.get(AlCodec::KEY_BIT_RATE, INT32_MIN));
    AlCodec::kMediaType type = static_cast<AlCodec::kMediaType>(
            format.get(AlCodec::KEY_MEDIA_TYPE,
                       (int32_t) AlCodec::kMediaType::UNKNOWN));
    if (AlCodec::kMediaType::AUDIO == type) {
        stream->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
        setInt32Parameter(stream->codecpar->sample_rate,
                          format.get(AlCodec::KEY_SAMPLE_RATE, INT32_MIN));
        setInt32Parameter(stream->codecpar->channels,
                          format.get(AlCodec::KEY_CHANNELS, INT32_MIN));
        setInt32Parameter(stream->codecpar->frame_size,
                          format.get(AlCodec::KEY_FRAME_SIZE, INT32_MIN));
        stream->codecpar->channel_layout = static_cast<uint64_t>(
                av_get_default_channel_layout(stream->codecpar->channels));
        stream->codecpar->format = AV_SAMPLE_FMT_FLTP;
//        stream->time_base = {1, stream->codecpar->sample_rate};
    } else if (AlCodec::kMediaType::VIDEO == type) {
        stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
        setInt32Parameter(stream->codecpar->width,
                          format.get(AlCodec::KEY_WIDTH, INT32_MIN));
        setInt32Parameter(stream->codecpar->height,
                          format.get(AlCodec::KEY_HEIGHT, INT32_MIN));
        stream->codecpar->format = AV_PIX_FMT_YUV420P;
//        stream->time_base = {1, format.get(HwAbsCodec::KEY_FPS, INT32_MIN)};
    }
    tracks.push_back(stream);
    return tracks.empty() ? TRACK_NONE : tracks.size() - 1;
}

bool HwFFMuxer::_configure(int32_t track, HwPacket *pkt) {
    AVStream *stream = tracks[track];
    if (nullptr == stream || nullptr == pkt) {
        return false;
    }
    /// 有些Codec可能不需要extra data，所以不需要copy，仅仅作为一个标记即可
    if (pkt->size() > 0) {
        stream->codecpar->extradata_size = static_cast<int>(pkt->size());
        stream->codecpar->extradata = static_cast<uint8_t *>(av_mallocz(pkt->size()));
        memcpy(stream->codecpar->extradata, pkt->data(), stream->codecpar->extradata_size);
    }
    AlLogI(TAG, "track(%d), extra data size(%d)", stream->index, stream->codecpar->extradata_size);
    mTrackConfigured[track] = true;
    for (int i = 0; i < mTrackConfigured.size(); ++i) {
        if (!mTrackConfigured[i]) {
            break;
        }
        if (mTrackConfigured[i] && i == mTrackConfigured.size() - 1) {
            if (avio_open2(&pFormatCtx->pb, filePath.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) <
                0) {
                AlLogE(TAG, "failed to open output file!");
                release();
                return false;
            }
            int ret = avformat_write_header(pFormatCtx, nullptr);
            if (ret < 0) {
                AlLogE(TAG, "failed to write header, %s", strerror(AVUNERROR(ret)));
                release();
                return false;
            }
            state = kState::RUNNING;
            AlLogI(TAG, "enter state RUNNING");
        }
    }
    return true;
}

HwResult HwFFMuxer::write(int32_t track, HwPacket *pkt) {
    if (track > tracks.size() - 1 ||
        track <= TRACK_NONE || nullptr == pkt) {
        return Hw::FAILED;
    }
    if (kState::CONFIGURED == state && pkt->getFlags() & AlMediaDef::FLAG_CONFIG) {
        HwResult ret = Hw::FAILED;
        if (_configure(track, pkt)) {
            ret = Hw::SUCCESS;
        }
        return ret;
    }
    if (kState::RUNNING != state) {
        return Hw::FAILED;
    }
    pkt->ref(&avPacket);
    avPacket->stream_index = tracks[track]->index;
    AVRational tb = tracks[track]->time_base;
    // pts / cq * bq
    av_packet_rescale_ts(avPacket, AV_TIME_BASE_Q, tb);
    if (tracks[track]->cur_dts >= avPacket->dts) {
        AlLogE(TAG,
               "will failed track(%d) cur_dts(%lld), pts(%lld), dts(%lld), idx(%d), flags(%s), try reset correctly.",
               track, tracks[track]->cur_dts, avPacket->pts, avPacket->dts, avPacket->stream_index,
               pkt->getFlagsStr().c_str());
        avPacket->dts += 1;
    }
    int ret = av_interleaved_write_frame(pFormatCtx, avPacket);
    if (0 != ret) {
        AlLogE(TAG, "failed(track %d): %s", track, strerror(AVUNERROR(ret)));
        return Hw::FAILED;
    }
    return Hw::SUCCESS;
}