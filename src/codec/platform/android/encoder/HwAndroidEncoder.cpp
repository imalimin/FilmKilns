/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAndroidEncoder.h"
#include "Logcat.h"
#include <media/NdkImage.h>

HwAndroidEncoder::HwAndroidEncoder() : HwAbsEncoder() {

}

HwAndroidEncoder::~HwAndroidEncoder() {
    release();
}

bool HwAndroidEncoder::prepare(string path, int width, int height) {
    this->path = path;
    this->width = width;
    this->height = height;
    this->keyFrameBuf = HwBuffer::alloc(static_cast<size_t>(width * height * 3 / 2));
    return configure() && configureMuxer();
}

HwResult HwAndroidEncoder::write(HwAbsMediaFrame *frame) {
    if (Hw::SUCCESS == push(frame)) {
        if (Hw::SUCCESS == pop()) {
            return Hw::SUCCESS;
        }
    }
    return Hw::FAILED;
}

bool HwAndroidEncoder::stop() {
    if (codec) {
        flush();
        AMediaCodec_stop(codec);
        AMediaCodec_delete(codec);
        codec = nullptr;
    }
    if (pFormatCtx) {
        av_write_trailer(pFormatCtx);
    }
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
    // stop ffmpeg
    if (pVideoStream && pVideoStream->codec) {
        avcodec_close(pVideoStream->codec);
        pVideoStream->codec = nullptr;
        pVideoStream = nullptr;
    }
    if (pFormatCtx) {
        avio_close(pFormatCtx->pb);
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
    if (configBuf) {
        delete configBuf;
        configBuf = nullptr;
    }
    if (keyFrameBuf) {
        delete keyFrameBuf;
        keyFrameBuf = nullptr;
    }
    return true;
}

void HwAndroidEncoder::release() {
    stop();
}

bool HwAndroidEncoder::configure() {
    AMediaFormat *format = AMediaFormat_new();
    AMediaFormat_setString(format, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_WIDTH, width);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_HEIGHT, height);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, COLOR_FormatYUV420Flexible);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_BIT_RATE, width * height * 3);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_FRAME_RATE, 30);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 10);
    const char *mime;
    AMediaFormat_getString(format, AMEDIAFORMAT_KEY_MIME, &mime);
    codec = AMediaCodec_createEncoderByType(mime);
    if (!codec) {
        Logcat::e("HWVC", "HwAndroidEncoder codec alloc failed");
        AMediaFormat_delete(format);
        return false;
    }
    media_status_t ret = AMediaCodec_configure(codec, format, nullptr, nullptr,
                                               AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidEncoder codec configure failed(%d)", ret);
        AMediaFormat_delete(format);
        return false;
    }
    ret = AMediaCodec_start(codec);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidEncoder codec start failed(%d)", ret);
        AMediaFormat_delete(format);
        return false;
    }
    AMediaFormat_delete(format);
    return true;
}

bool HwAndroidEncoder::configureMuxer() {
    av_register_all();
    avformat_alloc_output_context2(&pFormatCtx, NULL, "mp4", path.c_str());
    pFormatCtx->oformat->video_codec = AV_CODEC_ID_H264;
    if (avio_open2(&pFormatCtx->pb, path.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) {
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to open output file!");
        return false;
    }
    av_dict_set(&pFormatCtx->metadata, "comment", "hwvc", 0);
    pVideoStream = avformat_new_stream(pFormatCtx, 0);
    if (nullptr == pVideoStream) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to create video stream!");
        return false;
    }
    pVideoStream->time_base = outTimeBase;
    AVCodecContext *pCodecCtx = pVideoStream->codec;
    configureCodec(pCodecCtx);
    /**
     * Important.If not set this, the output will be fail.
     */
    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        pVideoStream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    AVCodec *pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not find %d codec!",
                  pCodecCtx->codec_id);
        return false;
    }
    AVDictionary *param = nullptr;
    if (AV_CODEC_ID_H264 == pCodecCtx->codec_id) {
        av_dict_set(&param, "preset", "superfast", 0);
        av_dict_set(&param, "tune", "zerolatency", 0);
        av_dict_set(&param, "crf", "15", 0);
        //av_dict_set(param, "profile", "main", 0)
    }
    if (avcodec_open2(pCodecCtx, pCodec, &param) < 0) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not open %d codec!",
                  pCodecCtx->codec_id);
        return false;
    }
    avformat_write_header(pFormatCtx, nullptr);
    avPacket = av_packet_alloc();
    return true;
}

void HwAndroidEncoder::configureCodec(AVCodecContext *ctx) {
    ctx->codec_id = pFormatCtx->oformat->video_codec;
    ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    ctx->width = width;
    ctx->height = height;
    ctx->bit_rate = width * height * 3;
    ctx->profile = FF_PROFILE_H264_HIGH;
    ctx->gop_size = 150;
    ctx->time_base = outTimeBase;

    ctx->thread_count = 0;
    ctx->qmin = 10;
    ctx->qmax = 30;
    ctx->max_b_frames = 3;
}

HwResult HwAndroidEncoder::push(HwAbsMediaFrame *frame) {
    ssize_t bufIdx = AMediaCodec_dequeueInputBuffer(codec, 2000);
    if (bufIdx >= 0) {
        if (!frame) {
            media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx, 0, 0, 0,
                                                                 AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
            if (media_status_t::AMEDIA_OK != result) {
                Logcat::e("HWVC", "HwAndroidEncoder queue eof buffer failed(%d)", result);
            }
            return Hw::SUCCESS;
        }
        size_t bufSize = frame->getBufferSize();
        int64_t pts = frame->getPts();
        auto buf = AMediaCodec_getInputBuffer(codec, bufIdx, &bufSize);
        if (bufSize > 0) {
            memcpy(buf, frame->getBuffer()->getData(), bufSize);
        }
        media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx,
                                                             0, bufSize,
                                                             pts, 0);
        if (media_status_t::AMEDIA_OK != result) {
            Logcat::e("HWVC", "HwAndroidEncoder queue input buffer failed(%d)", result);
        }
        return Hw::SUCCESS;
    }
    return Hw::FAILED;
}

HwResult HwAndroidEncoder::pop() {
    AMediaCodecBufferInfo info;
    ssize_t bufIdx = AMediaCodec_dequeueOutputBuffer(codec, &info, 2000);
    switch (bufIdx) {
        case AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED: {
            Logcat::i("HWVC", "HwAndroidEncoder AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED");
            break;
        }
        case AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED: {
            Logcat::i("HWVC", "HwAndroidEncoder AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED");
            auto *format = AMediaCodec_getOutputFormat(codec);
            uint8_t *sps = nullptr;
            size_t spsSize = 0;
            uint8_t *pps = nullptr;
            size_t ppsSize = 0;
            AMediaFormat_getBuffer(format, "csd-0", reinterpret_cast<void **>(&sps), &spsSize);
            AMediaFormat_getBuffer(format, "csd-1", reinterpret_cast<void **>(&pps), &ppsSize);
            return pop();
        }
        case AMEDIACODEC_INFO_TRY_AGAIN_LATER: {
            Logcat::i("HWVC", "HwAndroidEncoder AMEDIACODEC_INFO_TRY_AGAIN_LATER");
            break;
        }
        case AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM: {
            Logcat::i("HWVC", "HwAndroidEncoder AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM");
            break;
        }
        default: {
            if (bufIdx < 0) break;
            size_t inBufSize = 0;
            auto buf = AMediaCodec_getOutputBuffer(codec, bufIdx, &inBufSize);
            if (buf) {
                bool wrote = false;
                auto endOfStream = info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM;
                if (endOfStream == 0) {//如果没有收到BUFFER_FLAG_END_OF_STREAM信号，则代表输出数据时有效的
                    if (info.size > 0) {
                        if (info.flags & BUFFER_FLAG_CODEC_CONFIG) {//  config
                            if (configBuf && configBuf->size() != info.size) {
                                delete configBuf;
                                configBuf = nullptr;
                            }
                            if (!configBuf) {
                                configBuf = HwBuffer::alloc(info.size);
                            }
                            memcpy(configBuf->getData(), buf, configBuf->size());
                            wrote = false;
                        } else {
                            av_init_packet(avPacket);
                            avPacket->stream_index = pVideoStream->index;
                            avPacket->pts = av_rescale_q(info.presentationTimeUs,
                                                         (AVRational) {1, AV_TIME_BASE},
                                                         pVideoStream->time_base);
                            avPacket->dts = avPacket->pts;
                            avPacket->duration = 0;
                            if (info.flags & BUFFER_FLAG_KEY_FRAME) {// key frame
                                memcpy(keyFrameBuf->getData(),
                                       configBuf->getData(),
                                       configBuf->size());
                                memcpy(keyFrameBuf->getData() + configBuf->size(), buf, info.size);
                                avPacket->data = keyFrameBuf->getData();
                                avPacket->size = configBuf->size() + info.size;
                                write();
                            } else {
                                avPacket->data = buf;
                                avPacket->size = info.size;
                                write();
                            }
                            av_packet_unref(avPacket);
                            wrote = true;
                        }
                    }
                }
                //缓冲区使用完后必须把它还给MediaCodec，以便再次使用，至此一个流程结束，再次循环
                AMediaCodec_releaseOutputBuffer(codec, bufIdx, info.size != 0);
                return wrote ? Hw::SUCCESS : Hw::FAILED;
            }
        }
    }
    return Hw::FAILED;
}

HwResult HwAndroidEncoder::write() {
    int ret = av_write_frame(pFormatCtx, avPacket);
    return Hw::FAILED;
}

void HwAndroidEncoder::flush() {
    push(nullptr);
    while (Hw::SUCCESS == pop()) {

    }
}
