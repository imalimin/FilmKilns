/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAndroidAudioDecoder.h"
#include "../include/HwAudioFrame.h"

HwAndroidAudioDecoder::HwAndroidAudioDecoder() : AbsAudioDecoder() {
    hwFrameAllocator = new HwFrameAllocator();
}

HwAndroidAudioDecoder::~HwAndroidAudioDecoder() {
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
    if (pFormatCtx) {
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
    if (codec) {
        AMediaCodec_stop(codec);
        AMediaCodec_delete(codec);
        codec = nullptr;
    }
    if (outHwFrame) {
        delete outHwFrame;
        outHwFrame = nullptr;
    }
    if (hwFrameAllocator) {
        delete hwFrameAllocator;
        hwFrameAllocator = nullptr;
    }
};

bool HwAndroidAudioDecoder::prepare(string path) {
    Logcat::i("HWVC", "HwAndroidAudioDecoder::prepare: %s", path.c_str());
    this->path = path;
    av_register_all();
    pFormatCtx = avformat_alloc_context();
    //打开输入视频文件
    if (avformat_open_input(&pFormatCtx, path.c_str(), NULL, NULL) != 0) {
        Logcat::e("HWVC", "HwAndroidAudioDecoder Couldn't open input stream.");
        return false;
    }
    //获取视频文件信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        Logcat::e("HWVC", "HwAndroidAudioDecoder Couldn't find stream information.");
        return -1;
    }
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (-1 == track &&
            AVMediaType::AVMEDIA_TYPE_AUDIO == pFormatCtx->streams[i]->codecpar->codec_type) {
            track = i;
        }
    }
    if (track < 0 || !prepareCodec()) {
        Logcat::e("HWVC", "HwAndroidAudioDecoder ******** Open audio track failed. *********");
        return false;
    }
    avPacket = av_packet_alloc();
    outHwFrame = new HwAudioFrame(nullptr,
                                  HwAbsMediaFrame::convertToAudioFrameFormat(
                                          static_cast<AVSampleFormat>(getSampleFormat())),
                                  getChannels(),
                                  getSampleHz(),
                                  getSamplesPerBuffer());
    Logcat::e("HWVC",
              "HwAndroidAudioDecoder::prepare(duration=%lld channels=%d, sampleHz=%d, frameSize=%d)",
              getAudioDuration(), getChannels(), getSampleHz(), getSamplesPerBuffer());
    return true;
}

bool HwAndroidAudioDecoder::prepareCodec() {
    AMediaFormat *format = AMediaFormat_new();
    AMediaFormat_setString(format, AMEDIAFORMAT_KEY_MIME, "audio/mp4a-latm");
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_CHANNEL_COUNT, getChannels());
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_SAMPLE_RATE, getSampleHz());
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_AAC_PROFILE, getProfile());
    AMediaFormat_setBuffer(format, "csd-0", pFormatCtx->streams[track]->codecpar->extradata,
                           pFormatCtx->streams[track]->codecpar->extradata_size);
    const char *mime;
    AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &mime);
    codec = AMediaCodec_createDecoderByType(mime);
    if (!codec) {
        Logcat::e("HWVC", "HwAndroidAudioDecoder codec alloc failed");
        AMediaFormat_delete(format);
        return false;
    }
    media_status_t ret = AMediaCodec_configure(codec, format, nullptr, nullptr, 0);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidAudioDecoder codec configure failed(%d)", ret);
        AMediaFormat_delete(format);
        return false;
    }
    ret = AMediaCodec_start(codec);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidAudioDecoder codec start failed(%d)", ret);
        AMediaFormat_delete(format);
        return false;
    }
    AMediaFormat_delete(format);
    return true;
}

int HwAndroidAudioDecoder::getProfile() {
    int profile = pFormatCtx->streams[track]->codecpar->profile;
    switch (profile) {
        case FF_PROFILE_AAC_MAIN:
            return AACObjectMain;
        case FF_PROFILE_AAC_LOW:
            return AACObjectLC;
        case FF_PROFILE_AAC_SSR:
            return AACObjectSSR;
        case FF_PROFILE_AAC_LTP:
            return AACObjectLTP;
        case FF_PROFILE_AAC_HE:
            return AACObjectHE;
        case FF_PROFILE_AAC_HE_V2:
            return AACObjectHE;
        case FF_PROFILE_AAC_LD:
            return AACObjectLD;
        case FF_PROFILE_AAC_ELD:
            return AACObjectELD;
        case FF_PROFILE_MPEG2_AAC_LOW:
            return AACObjectHE;
        case FF_PROFILE_MPEG2_AAC_HE:
            return AACObjectHE;
        default:
            return AACObjectMain;
    }
}

HwResult HwAndroidAudioDecoder::grab(HwAbsMediaFrame **frame) {
    while (true) {
        HwBuffer *buffer = HwBuffer::alloc(8192);
        size_t count = 0;
        int duration = 0;
        int64_t pts = 0;
        readPkgLock.lock();
        while (!eof) {
            int ret = av_read_frame(pFormatCtx, avPacket);
            if (0 == ret) {
                if (track == avPacket->stream_index) {
                    if (0 == pts) {
                        pts = av_rescale_q_rnd(avPacket->pts,
                                               pFormatCtx->streams[track]->time_base,
                                               outTimeBase,
                                               AV_ROUND_NEAR_INF);
                    }
                    if (1 == avPacket->flags) {
                        memcpy(buffer->getData() + count, avPacket->data, avPacket->size);
                        count += avPacket->size;
                        duration += avPacket->duration;
                    }
                }
                av_packet_unref(avPacket);//Or av_free_packet?
                if (duration >= 21334) {
                    break;
                }
                continue;
            }
            av_packet_unref(avPacket);//Or av_free_packet?
            if (AVERROR_EOF == ret) {
                eof = true;
            }
            break;
        }
        readPkgLock.unlock();
        ssize_t bufIdx = AMediaCodec_dequeueInputBuffer(codec, 2000);
        if (bufIdx >= 0) {
            size_t bufSize = 0;
            auto buf = AMediaCodec_getInputBuffer(codec, bufIdx, &bufSize);
            if (bufSize > 0) {
                bufSize = count;
                memcpy(buf, buffer->getData(), bufSize);
            }
            media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx, 0, bufSize, pts,
                                                                 eof
                                                                 ? AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM
                                                                 : 0);
            if (media_status_t::AMEDIA_OK != result) {
                Logcat::e("HWVC", "HwAndroidAudioDecoder queue input buffer failed(%d)", result);
            }
        }
        AMediaCodecBufferInfo info;
        auto status = AMediaCodec_dequeueOutputBuffer(codec, &info, 2000);
        if (status >= 0 && !(info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM)) {
            size_t bufSize = 0;
            auto buf = AMediaCodec_getOutputBuffer(codec, status, &bufSize);
            memcpy(outHwFrame->getBuffer()->getData(), buf,
                   min(bufSize, outHwFrame->getBuffer()->size()));
            AMediaCodec_releaseOutputBuffer(codec, status, info.size != 0);
            *frame = outHwFrame;
            return Hw::MEDIA_SUCCESS;
        } else if (status == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED) {
            Logcat::i("HWVC", "HwAndroidAudioDecoder output buffers changed");
        } else if (status == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
            auto format = AMediaCodec_getOutputFormat(codec);
            Logcat::i("HWVC", "HwAndroidAudioDecoder output format(%s) changed.",
                      AMediaFormat_toString(format));
            AMediaFormat_delete(format);
        } else if (status == AMEDIACODEC_INFO_TRY_AGAIN_LATER) {
            Logcat::i("HWVC", "HwAndroidAudioDecoder no output buffer right now");
            return Hw::MEDIA_WAIT;
        } else {
            Logcat::i("HWVC", "HwAndroidAudioDecoder unexpected info code: %zd", status);
            return Hw::MEDIA_WAIT;
        }

        //如果缓冲区中既没有音频也没有视频，并且已经读取完文件，则播放完了
        if (eof) {
            Logcat::i("HWVC", "HwAndroidAudioDecoder::grab EOF");
            return Hw::MEDIA_EOF;
        }
    }
}

void HwAndroidAudioDecoder::seek(int64_t us) {

}

int HwAndroidAudioDecoder::getChannels() {
    return pFormatCtx->streams[track]->codecpar->channels;
}

int HwAndroidAudioDecoder::getSampleHz() {
    return pFormatCtx->streams[track]->codecpar->sample_rate;
}

int HwAndroidAudioDecoder::getSampleFormat() {
    return pFormatCtx->streams[track]->codecpar->format;
}

int HwAndroidAudioDecoder::getSamplesPerBuffer() {
    return pFormatCtx->streams[track]->codecpar->frame_size;
}

int64_t HwAndroidAudioDecoder::getAudioDuration() {
    if (aDurationInUs >= 0) {
        return aDurationInUs;
    }
    aDurationInUs = pFormatCtx->streams[track]->duration;
    aDurationInUs = av_rescale_q_rnd(aDurationInUs,
                                     pFormatCtx->streams[track]->time_base,
                                     pFormatCtx->streams[track]->codec->time_base,
                                     AV_ROUND_NEAR_INF);
    aDurationInUs = av_rescale_q_rnd(aDurationInUs,
                                     pFormatCtx->streams[track]->codec->time_base,
                                     outTimeBase,
                                     AV_ROUND_NEAR_INF);
    return aDurationInUs;
}

int64_t HwAndroidAudioDecoder::getDuration() {
    return -1;
}