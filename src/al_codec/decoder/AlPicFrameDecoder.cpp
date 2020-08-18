/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPicFrameDecoder.h"
#include "AlLogcat.h"
#include "HwVideoFrame.h"
#include "platform/android/core/AlFFAndroidDec.h"

#define TAG "AlPicFrameDecoder"

AlPicFrameDecoder::AlPicFrameDecoder() : AbsVideoDecoder() {
    hwFrameAllocator = new HwFrameAllocator();
}

AlPicFrameDecoder::~AlPicFrameDecoder() {
    if (vPacket) {
        av_packet_free(&vPacket);
        vPacket = nullptr;
    }
    if (vFinalFrame) {
        av_frame_free(&vFinalFrame);
        vFinalFrame = nullptr;
    }
    if (vFrame) {
        av_frame_free(&vFrame);
        vFrame = nullptr;
    }
    if (vCtx) {
        avcodec_close(vCtx);
        vCtx = nullptr;
    }
    if (pFormatCtx) {
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
    if (sCtx) {
        sws_freeContext(sCtx);
        sCtx = nullptr;
    }
    if (oHwFrame) {
        oHwFrame->recycle();
        oHwFrame = nullptr;
    }
    if (bsf) {
        av_bsf_free(&bsf);
    }
    delete hwFrameAllocator;
    hwFrameAllocator = nullptr;
}

bool AlPicFrameDecoder::prepare(string path) {
    AlFFUtils::init();
#if defined(__AL_DEBUG__)
    AlFFUtils::showInfo();
#endif
    this->path = path;
    pFormatCtx = avformat_alloc_context();
    //打开输入视频文件
    if (avformat_open_input(&pFormatCtx, path.c_str(), NULL, NULL) != 0) {
        AlLogE(TAG, "failed. Couldn't open input stream.");
        return false;
    }

    vTrack = av_find_best_stream(pFormatCtx, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (vTrack < 0) {
        AlLogE(TAG, "failed. Couldn't find video track.");
        return false;
    }

    //获取视频文件信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        AlLogE(TAG, "failed. Couldn't find stream information.");
        return false;
    }
    AVCodec *codec = nullptr;
    if (AV_CODEC_ID_H264 == pFormatCtx->streams[vTrack]->codecpar->codec_id) {
        codec = avcodec_find_decoder_by_name("h264_mediacodec");
//        codec = avcodec_find_decoder(pFormatCtx->streams[vTrack]->codecpar->codec_id);
    } else {
        codec = avcodec_find_decoder(pFormatCtx->streams[vTrack]->codecpar->codec_id);
    }
    if (nullptr == codec) {
        AlLogE(TAG, "failed. Couldn't find codec.");
        return false;
    }
    vCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(vCtx, pFormatCtx->streams[vTrack]->codecpar);
    if (avcodec_open2(vCtx, codec, NULL) < 0) {
        AlLogE(TAG, "failed. Couldn't open codec.");
        return false;
    }
    vPacket = av_packet_alloc();
    vFrame = av_frame_alloc();
    auto format = pFormatCtx->streams[vTrack]->codecpar->format;
    if (AVPixelFormat::AV_PIX_FMT_YUV420P != format) {
        AlLogW(TAG, "This track`s pixel format(%d) is not yuv420p", format);
        _setupSwr();
    }
//    _setupBSF();
    return true;
}

int AlPicFrameDecoder::width() {
    if (!pFormatCtx) return 0;
    return pFormatCtx->streams[vTrack]->codecpar->width;
}

int AlPicFrameDecoder::height() {
    if (!pFormatCtx) return 0;
    return pFormatCtx->streams[vTrack]->codecpar->height;
}

void AlPicFrameDecoder::setOutSampleFormat(HwSampleFormat format) {

}

int AlPicFrameDecoder::getChannels() {
    return 0;
}

int AlPicFrameDecoder::getSampleHz() {
    return 0;
}

int AlPicFrameDecoder::getSampleFormat() {
    return 0;
}

int AlPicFrameDecoder::getSamplesPerBuffer() {
    return 0;
}

void AlPicFrameDecoder::seek(int64_t us, AbsDecoder::kSeekMode mode) {
    mSeekAction.put("mode", (int32_t) mode);
    mSeekAction.put("time", us);
    eof = false;
}

int32_t AlPicFrameDecoder::_handleAction() {
    auto timeInUS = mSeekAction.get("time", INT64_MIN);
    if (timeInUS >= 0) {
        auto mode = (AbsDecoder::kSeekMode) mSeekAction.get("mode",
                                                            static_cast<int32_t>(AbsDecoder::kSeekMode::BACKWARD));
        int ret = -1;
        if (!_inCurGop(timeInUS)) {
            avcodec_flush_buffers(vCtx);
            ret = avformat_seek_file(pFormatCtx, vTrack, INT64_MIN,
                                     av_rescale_q_rnd(timeInUS,
                                                      oRational,
                                                      pFormatCtx->streams[vTrack]->time_base,
                                                      AV_ROUND_NEAR_INF), INT64_MAX,
                                     AVSEEK_FLAG_BACKWARD);
            if (ret < 0) {
                AlLogE(TAG, "failed");
                return 0;
            }
        } else {
            AlLogD(TAG, "In current gop.");
        }
        while (AbsDecoder::kSeekMode::EXACT == mode && timeInUS > 0) {
            ret = av_read_frame(pFormatCtx, vPacket);
            if (0 == ret && vTrack == vPacket->stream_index) {
                avcodec_send_packet(vCtx, vPacket);
                lastPktPts = vPacket->pts;
            }
            av_packet_unref(vPacket);
            if (AVERROR_EOF == ret) {
                eof = true;
                mSeekAction.put("time", INT64_MIN);
                return AlMediaDef::FLAG_SEEK_DONE | AlMediaDef::FLAG_EOF;
            }
            int64_t pts = -1;
            if (0 == avcodec_receive_frame(vCtx, vFrame)) {
                pts = av_rescale_q_rnd(vFrame->pts,
                                       pFormatCtx->streams[vTrack]->time_base,
                                       oRational,
                                       AV_ROUND_NEAR_INF);
                av_frame_unref(vFrame);

            }
            if (pts >= timeInUS) {
                AlLogD(TAG, "Seek %" PRId64 ", target %" PRId64 ", %d", pts, timeInUS,
                       vCtx->gop_size);
                break;
            }
        }
        mSeekAction.put("time", INT64_MIN);
        AlLogI(TAG, "seek: %lld", timeInUS);
        return AlMediaDef::FLAG_SEEK_DONE;
    }
    return 0;
}

HwResult AlPicFrameDecoder::grab(HwAbsMediaFrame **frame) {
    auto flags = _handleAction();
    if (flags & AlMediaDef::FLAG_SEEK_DONE || flags & AlMediaDef::FLAG_EOF) {
        if (oHwFrame) {
            oHwFrame->recycle();
        }
        oHwFrame = new HwVideoFrame(hwFrameAllocator, HwFrameFormat::HW_FMT_NONE, 0, 0);
        oHwFrame->setFlags(flags);
        *frame = oHwFrame;
        return Hw::FAILED;
    }
    while (true) {
        int ret = av_read_frame(pFormatCtx, vPacket);
        if (0 == ret && vTrack == vPacket->stream_index) {
            avcodec_send_packet(vCtx, vPacket);
            lastPktPts = vPacket->pts;
        }
        av_packet_unref(vPacket);
        if (AVERROR_EOF == ret) {
            eof = true;
        }
        //尝试去缓冲区中获取解码完成的视频帧
        if (0 == avcodec_receive_frame(vCtx, vFrame)) {
            vFrame->pts = av_rescale_q_rnd(vFrame->pts,
                                           pFormatCtx->streams[vTrack]->time_base,
                                           oRational,
                                           AV_ROUND_NEAR_INF);
//            AlLogD(TAG, "format %d, %d, %d", pFormatCtx->streams[vTrack]->codecpar->format, vFrame->format, AV_PIX_FMT_NV12);
            if (oHwFrame) {
                oHwFrame->recycle();
            }
            oHwFrame = hwFrameAllocator->ref(_doSwr(vFrame));
            *frame = oHwFrame;
//            AlLogD(TAG, "%d x %d, %d", vFrame->width, vFrame->height, vFrame->format);
            av_frame_unref(vFrame);
            return Hw::MEDIA_SUCCESS;
        }
        if (eof) {
            AlLogI(TAG, "end");
            if (oHwFrame) {
                oHwFrame->recycle();
            }
            oHwFrame = new HwVideoFrame(hwFrameAllocator, HwFrameFormat::HW_FMT_NONE, 0, 0);
            oHwFrame->setFlags(AlMediaDef::FLAG_EOF);
            *frame = oHwFrame;
            return Hw::MEDIA_EOF;
        }
    }
}

int64_t AlPicFrameDecoder::getVideoDuration() {
    if (AV_NOPTS_VALUE == pFormatCtx->streams[vTrack]->duration) {
        auto r = pFormatCtx->streams[vTrack]->r_frame_rate;
        return pFormatCtx->streams[vTrack]->codec_info_nb_frames * r.den * AV_TIME_BASE / r.num;
    }
    return av_rescale_q_rnd(pFormatCtx->streams[vTrack]->duration,
                            pFormatCtx->streams[vTrack]->time_base,
                            oRational,
                            AV_ROUND_NEAR_INF);
}

int64_t AlPicFrameDecoder::getAudioDuration() {
    return 0;
}

int64_t AlPicFrameDecoder::getDuration() {
    return getVideoDuration();
}

int64_t AlPicFrameDecoder::getAudioStartTime() {
    return 0;
}

void AlPicFrameDecoder::start() {

}

void AlPicFrameDecoder::pause() {

}

void AlPicFrameDecoder::stop() {

}

void AlPicFrameDecoder::_setupSwr() {
    auto format = AV_PIX_FMT_RGBA;
    vFinalFrame = av_frame_alloc();
    vFinalFrame->width = vCtx->width;
    vFinalFrame->height = vCtx->height;
    vFinalFrame->format = format;
    sCtx = sws_getContext(vCtx->width, vCtx->height,
                          (AVPixelFormat) pFormatCtx->streams[vTrack]->codecpar->format,
                          vFinalFrame->width, vFinalFrame->height, format,
                          SWS_BICUBIC, nullptr, nullptr, nullptr);
    auto *buf = (uint8_t *) av_malloc(
            avpicture_get_size(format, vFinalFrame->width, vFinalFrame->height));
    int ret = av_image_fill_arrays(vFinalFrame->data, vFinalFrame->linesize, buf, format,
                                   vFinalFrame->width, vFinalFrame->height, 1);
}

AVFrame *AlPicFrameDecoder::_doSwr(AVFrame *src) {
    if (sCtx && vFinalFrame) {
        int ret = sws_scale(sCtx, src->data, src->linesize, 0, src->height,
                            vFinalFrame->data, vFinalFrame->linesize);
        vFinalFrame->pts = src->pts;
        vFinalFrame->pkt_dts = src->pkt_dts;
        vFinalFrame->pkt_duration = src->pkt_duration;
        return vFinalFrame;
    }
    return src;
}

void AlPicFrameDecoder::_setupBSF() {
    const AVBitStreamFilter *filter = av_bsf_get_by_name("h264_mp4toannexb");
    if (nullptr == filter) {
        AlLogE(TAG, "failed.");
    }
    int ret = av_bsf_alloc(filter, &bsf);
    if (0 != ret) {
        AlLogE(TAG, "failed.");
    }
    ret = avcodec_parameters_from_context(bsf->par_in, vCtx);
    if (0 != ret) {
        AlLogE(TAG, "failed.");
    }
    ret = av_bsf_init(bsf);
    if (ret < 0) {
        AlLogE(TAG, "failed.");
    }
    if (vCtx->extradata) {
        av_free(vCtx->extradata);
        vCtx->extradata = (uint8_t *) av_malloc((size_t) bsf->par_out->extradata_size);
        vCtx->extradata_size = bsf->par_out->extradata_size;

        memcpy(vCtx->extradata, bsf->par_out->extradata, vCtx->extradata_size);
    }
}

bool AlPicFrameDecoder::_inCurGop(int64_t timeInUS) {
    int64_t reqTimeInBase = av_rescale_q_rnd(timeInUS,
                                     oRational,
                                     pFormatCtx->streams[vTrack]->time_base,
                                     AV_ROUND_NEAR_INF);
    int64_t curTime = av_index_search_timestamp(pFormatCtx->streams[vTrack], lastPktPts, AVSEEK_FLAG_BACKWARD);
    int64_t reqTime = av_index_search_timestamp(pFormatCtx->streams[vTrack], reqTimeInBase, AVSEEK_FLAG_BACKWARD);
    return curTime == reqTime && reqTimeInBase > lastPktPts;
}
