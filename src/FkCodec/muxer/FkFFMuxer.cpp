/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkFFMuxer.h"
#include "FkFFUtils.h"
#include "libavutil/opt.h"

#define TAG "FkFFMuxer"

FK_IMPL_CLASS_TYPE(FkFFMuxer, FkAbsMuxer)

FkFFMuxer::FkFFMuxer() : FkAbsMuxer() {
}

FkFFMuxer::~FkFFMuxer() {
    release();
}

void FkFFMuxer::release() {
    if (pFormatCtx) {
        if (state >= kState::RUNNING && 0 == av_write_trailer(pFormatCtx)) {
            FkLogI(TAG, "close file success.");
        } else {
            FkLogE(TAG, "close file failed!");
        }
        if (!(pFormatCtx->flags & AVFMT_NOFILE) && pFormatCtx->pb) {
            if (0 != avio_closep(&pFormatCtx->pb)) {
                FkLogE(TAG, "close file failed!");
            }
        }
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
    avformat_network_deinit();
    tracks.clear();
    state = kState::IDL;
    FkLogI(TAG, "enter state IDL");
}

FkResult FkFFMuxer::configure(std::string _filePath, std::string _type) {
    FkAbsMuxer::configure(_filePath, _type);
    FkFFUtils::init();
    int ret = avformat_alloc_output_context2(&pFormatCtx, NULL, _type.c_str(), _filePath.c_str());
    if (ret < 0 || !pFormatCtx) {
        FkLogE(TAG, "failed %s", strerror(AVUNERROR(ret)));
        return FK_FAIL;
    }
    if (_type == "RTSP" || _type == "rtsp") {
        ret = avformat_network_init();
        pFormatCtx->flags |= AVFMT_NOFILE;
        FkLogI(TAG, "avformat_network_init: %d", ret);
    }
    av_dict_set(&pFormatCtx->metadata, "comment", "FilmKilns", 0);
    avPacket = av_packet_alloc();
    FkLogI(TAG, "configure: %s, %s", _type.c_str(), _filePath.c_str());
    return FK_OK;
}

FkResult FkFFMuxer::start() {
    mTrackConfigured.resize(tracks.size());
    for (int i = 0; i < mTrackConfigured.size(); ++i) {
        mTrackConfigured[i] = false;
    }
    state = kState::CONFIGURED;
    FkLogI(TAG, "enter state CONFIGURED");
    return FK_OK;
}

void FkFFMuxer::setInt32Parameter(int32_t &param, int32_t value) {
    if (INT32_MIN != value) {
        param = value;
    }
}

void FkFFMuxer::setInt64Parameter(int64_t &param, int64_t value) {
    if (INT64_MIN != value) {
        param = value;
    }
}

int32_t FkFFMuxer::addTrack(std::shared_ptr<FkBundle> format) {
    FkLogI(TAG, format->toString());
    AVStream *stream = avformat_new_stream(pFormatCtx, nullptr);
    if (!stream) {
        return TRACK_NONE;
    }

    // See avcodec_parameters_from_context
    stream->codecpar->codec_id = static_cast<AVCodecID>(format->get(FkCodec::KEY_CODEC_ID,
                                                                   INT32_MIN));
    setInt32Parameter(stream->codecpar->profile,
                      format->get(FkCodec::KEY_PROFILE, INT32_MIN));
    setInt32Parameter(stream->codecpar->level,
                      format->get(FkCodec::KEY_LEVEL, INT32_MIN));
    setInt64Parameter(stream->codecpar->bit_rate,
                      format->get(FkCodec::KEY_BIT_RATE, INT32_MIN));
    FkCodec::kMediaType type = static_cast<FkCodec::kMediaType>(
            format->get(FkCodec::KEY_MEDIA_TYPE,
                       (int32_t) FkCodec::kMediaType::UNKNOWN));
    if (FkCodec::kMediaType::AUDIO == type) {
        stream->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
        setInt32Parameter(stream->codecpar->sample_rate,
                          format->get(FkCodec::KEY_SAMPLE_RATE, INT32_MIN));
        setInt32Parameter(stream->codecpar->channels,
                          format->get(FkCodec::KEY_CHANNELS, INT32_MIN));
        setInt32Parameter(stream->codecpar->frame_size,
                          format->get(FkCodec::KEY_FRAME_SIZE, INT32_MIN));
        stream->codecpar->channel_layout = static_cast<uint64_t>(
                av_get_default_channel_layout(stream->codecpar->channels));
        stream->codecpar->format = AV_SAMPLE_FMT_FLTP;
//        stream->time_base = {1, stream->codecpar->sample_rate};
    } else if (FkCodec::kMediaType::VIDEO == type) {
        stream->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
        setInt32Parameter(stream->codecpar->width,
                          format->get(FkCodec::KEY_WIDTH, INT32_MIN));
        setInt32Parameter(stream->codecpar->height,
                          format->get(FkCodec::KEY_HEIGHT, INT32_MIN));
        stream->codecpar->format = AV_PIX_FMT_YUV420P;
//        stream->time_base = {1, format->get(FkCodec::KEY_FPS, INT32_MIN)};
    }
    tracks.push_back(stream);
    return tracks.empty() ? TRACK_NONE : tracks.size() - 1;
}

bool FkFFMuxer::_configure(int32_t track, FkPacket *pkt) {
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
    FkLogI(TAG, "track(%d), extra data size4(%d), %d,%d,%d,%d",
           stream->index, stream->codecpar->extradata_size,
           stream->codecpar->extradata[0],
           stream->codecpar->extradata[1],
           stream->codecpar->extradata[2],
           stream->codecpar->extradata[3]);
    if (stream->codecpar->extradata_size >= 8) {
        FkLogI(TAG, "track(%d), extra data size8, %d,%d,%d,%d",
               stream->index, stream->codecpar->extradata_size,
               stream->codecpar->extradata[4],
               stream->codecpar->extradata[5],
               stream->codecpar->extradata[6],
               stream->codecpar->extradata[7]);
    }
    mTrackConfigured[track] = true;
    for (int i = 0; i < mTrackConfigured.size(); ++i) {
        if (!mTrackConfigured[i]) {
            break;
        }
        if (mTrackConfigured[i] && i == mTrackConfigured.size() - 1) {
            if (!(pFormatCtx->flags & AVFMT_NOFILE)) {
                auto ret = avio_open(&pFormatCtx->pb, pFormatCtx->filename, AVIO_FLAG_WRITE);
                if (ret < 0) {
                    FkLogE(TAG, "failed to open output file %s: %s", strerror(AVUNERROR(ret)),
                           pFormatCtx->filename);
                    release();
                    return false;
                }
            }
            AVDictionary *dict = NULL;
            av_dict_set(&dict, "rtsp_transport","tcp",0);
            av_dict_set(&dict, "muxdelay", "0.1", 0);
            auto ret = avformat_write_header(pFormatCtx, &dict);
            if (ret < 0) {
                FkLogE(TAG, "failed to write header, %s", strerror(AVUNERROR(ret)));
                release();
                return false;
            }
            state = kState::RUNNING;
            FkLogI(TAG, "enter state RUNNING");
        }
    }
    return true;
}

FkResult FkFFMuxer::write(int32_t track, FkPacket *pkt) {
    if (track > tracks.size() - 1 ||
        track <= TRACK_NONE || nullptr == pkt) {
        return FK_FAIL;
    }
    if (kState::CONFIGURED == state && pkt->getFlags() & FK_CTL_FLAG_CONFIG) {
        FkResult ret = FK_FAIL;
        if (_configure(track, pkt)) {
            ret = FK_OK;
        }
        return ret;
    }
    if (kState::RUNNING != state) {
        return FK_INVALID_STATE;
    }
    pkt->ref(&avPacket);
    avPacket->stream_index = tracks[track]->index;
    AVRational tb = tracks[track]->time_base;
    // pts / cq * bq
    av_packet_rescale_ts(avPacket, AV_TIME_BASE_Q, tb);
    if (tracks[track]->cur_dts >= avPacket->dts) {
        FkLogE(TAG,
               "will failed track(%d) cur_dts(%lld), pts(%lld), dts(%lld), idx(%d), flags(%s), try reset correctly.",
               track, tracks[track]->cur_dts, avPacket->pts, avPacket->dts, avPacket->stream_index,
               pkt->getFlagsStr().c_str());
        avPacket->dts += 1;
    }
    if (track == 0 && pkt->getFlags() & FK_CTL_FLAG_KEY) {
        FkLogI(TAG, "%s: %d,%d,%d,%d", pkt->getFlagsStr().c_str(),
               pkt->data()[0], pkt->data()[1], pkt->data()[2], pkt->data()[3]);
    }
    int ret = av_interleaved_write_frame(pFormatCtx, avPacket);
    if (0 != ret) {
        FkLogE(TAG, "write failed(track %d, flags %s): %s(%d)",
               track,
               pkt->getFlagsStr().c_str(),
               strerror(AVUNERROR(ret)),
               ret);
        return FK_FAIL;
    }
    return FK_OK;
}