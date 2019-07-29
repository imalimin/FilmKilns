/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwFFmpegEncoder.h"
#include "Logcat.h"
#include "../include/HwVideoFrame.h"
#include "../include/HwAudioFrame.h"

HwFFmpegEncoder::HwFFmpegEncoder() : HwAbsVideoEncoder() {

}

HwFFmpegEncoder::~HwFFmpegEncoder() {
    release();
}

bool HwFFmpegEncoder::prepare(string path, int width, int height, HwSampleFormat audioFormat) {
    this->path = path;
    this->width = width;
    this->height = height;
    this->audioFormat = audioFormat;
    translator = new HwAudioTranslator(HwSampleFormat(HwFrameFormat::HW_SAMPLE_FLTP,
                                                      this->audioFormat.getChannels(),
                                                      this->audioFormat.getSampleRate()),
                                       this->audioFormat);
    return initialize();
}

bool HwFFmpegEncoder::initialize() {
    av_register_all();
    avformat_alloc_output_context2(&pFormatCtx, NULL, "mp4", path.c_str());
    av_dict_set(&pFormatCtx->metadata, "comment", "hwvc", 0);
    pFormatCtx->oformat->video_codec = AV_CODEC_ID_H264;
    if (AV_CODEC_ID_NONE != pFormatCtx->oformat->video_codec) {
        openVideoTrack();
    }
//    pFormatCtx->oformat->audio_codec = AV_CODEC_ID_AAC_LATM;
    if (AV_CODEC_ID_NONE != pFormatCtx->oformat->audio_codec) {
        openAudioTrack();
    }
    if (avio_open2(&pFormatCtx->pb, path.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr) < 0) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to open output file!");
        return false;
    }
    avformat_write_header(pFormatCtx, nullptr);
    avFrame = av_frame_alloc();
    avAudioFrame = av_frame_alloc();
    avPacket = av_packet_alloc();
    return true;
}

bool HwFFmpegEncoder::openVideoTrack() {
    AVCodec *pCodec = avcodec_find_encoder(pFormatCtx->oformat->video_codec);
    if (!pCodec) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not find %d codec!",
                  pFormatCtx->oformat->video_codec);
        return false;
    }
    pVideoStream = avformat_new_stream(pFormatCtx, nullptr);
    if (nullptr == pVideoStream) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to create video stream!");
        return false;
    }
//    pVideoStream->id = pFormatCtx->nb_streams - 1;
    pVideoStream->time_base = {1, 30};
    vCtx = avcodec_alloc_context3(pCodec);
    // Configure
    vCtx->codec_id = pFormatCtx->oformat->video_codec;
    vCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    vCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    vCtx->width = width;
    vCtx->height = height;
    vCtx->bit_rate = width * height * 3;
    vCtx->gop_size = 15;

    vCtx->time_base = pVideoStream->time_base;

//    pCodecCtx->ticks_per_frame = 2;
    vCtx->thread_count = 0;
    vCtx->qmin = 10;
    vCtx->qmax = 30;
    vCtx->max_b_frames = 2;
//    pCodecCtx->me_range = 16;
//    pCodecCtx->max_qdiff = 4;
    vCtx->codec = pCodec;
    /**
     * Important. If not set this, the output will be fail.
     */
    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        vCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    AVDictionary *param = nullptr;
    if (AV_CODEC_ID_H264 == vCtx->codec_id) {
        vCtx->profile = FF_PROFILE_H264_HIGH;
        av_dict_set_int(&param, "crf", quality, 0);  // or abr,qp
        av_dict_set(&param, "preset", "superfast", 0);
        //av_dict_set(param, "profile", "main", 0)
        av_dict_set(&param, "tune", "zerolatency", 0);
    }
    int ret = avcodec_open2(vCtx, pCodec, &param);
    av_dict_free(&param);
    if (ret < 0) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not open %d codec!",
                  vCtx->codec_id);
        return false;
    }
    ret = avcodec_parameters_from_context(pVideoStream->codecpar, vCtx);
    return true;
}

bool HwFFmpegEncoder::openAudioTrack() {
    AVCodec *pCodec = avcodec_find_encoder(pFormatCtx->oformat->audio_codec);
    if (!pCodec) {
        pAudioStream = nullptr;
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not find %d codec!",
                  pFormatCtx->oformat->audio_codec);
        return false;
    }
    pAudioStream = avformat_new_stream(pFormatCtx, 0);
    if (nullptr == pAudioStream) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize failed to create audio stream!");
        return false;
    }
//    pVideoStream->id = pFormatCtx->nb_streams - 1;
    int sampleRate = audioFormat.getSampleRate();
    pAudioStream->time_base = {1, sampleRate};
    aCtx = avcodec_alloc_context3(pCodec);
    aCtx->codec_id = pFormatCtx->oformat->audio_codec;
    aCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    aCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    aCtx->bit_rate = 64000;
    aCtx->sample_rate = sampleRate;
    aCtx->channels = this->audioFormat.getChannels();
    aCtx->channel_layout = static_cast<uint64_t>(
            av_get_default_channel_layout(aCtx->channels));
    aCtx->time_base = pAudioStream->time_base;
    aCtx->codec = pCodec;
    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        aCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    AVDictionary *param = nullptr;
    int ret = avcodec_open2(aCtx, pCodec, &param);
    av_dict_free(&param);
    if (ret < 0) {
        if (aCtx) {
            avcodec_close(aCtx);
            avcodec_free_context(&aCtx);
        }
        pAudioStream = nullptr;
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not open audio codec: %s",
                  strerror(AVUNERROR(ret)));
        return false;
    }
    ret = avcodec_parameters_from_context(pAudioStream->codecpar, aCtx);
    return true;
}

HwResult HwFFmpegEncoder::write(HwAbsMediaFrame *frame) {
    AVStream *stream = nullptr;
    AVCodecContext *ctx = nullptr;
    int64_t duration = 1;
    if (frame->isAudio() && pAudioStream) {
        // Ensure that the first frame is video.
        if (!firstVideoFrameWrite) {
            return Hw::FAILED;
        }
        stream = pAudioStream;
        ctx = aCtx;
        duration = static_cast<int64_t>(AV_TIME_BASE / ctx->sample_rate * frame->getBufferSize());
        HwAudioFrame *audioFrame = dynamic_cast<HwAudioFrame *>(frame);
        avAudioFrame->data[0] = audioFrame->getBuffer()->getData();
        avAudioFrame->linesize[0] = audioFrame->getBufferSize();
        avAudioFrame->format = HwAbsMediaFrame::convertAudioFrameFormat(frame->getFormat());
        avAudioFrame->sample_rate = audioFrame->getSampleRate();
        if (ctx->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE)
            avAudioFrame->nb_samples = 10000;
        else
            avAudioFrame->nb_samples = aCtx->frame_size;
        avAudioFrame->channels = audioFrame->getChannels();
        avAudioFrame->channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(
                audioFrame->getChannels()));
        sampleCount += avAudioFrame->nb_samples;
//        avFrame->pts = sampleCount;
        avAudioFrame->pts = av_rescale_q_rnd(frame->getPts(),
                                        {1, AV_TIME_BASE},
                                        pAudioStream->time_base,
                                        AV_ROUND_NEAR_INF);

        AVFrame *avFrame = nullptr;
        translator->translate(&avFrame, &avAudioFrame);
        avcodec_send_frame(ctx, avFrame);
    } else if (frame->isVideo() && pVideoStream) {
        frameCount += 1;
        stream = pVideoStream;
        ctx = vCtx;
        duration = 1;
        HwVideoFrame *videoFrame = dynamic_cast<HwVideoFrame *>(frame);
        int pixelCount = videoFrame->getWidth() * videoFrame->getHeight();
        avFrame->data[0] = videoFrame->getBuffer()->getData();
        avFrame->data[1] = videoFrame->getBuffer()->getData() + pixelCount;
        avFrame->data[2] = videoFrame->getBuffer()->getData() + pixelCount * 5 / 4;

        avFrame->linesize[0] = videoFrame->getWidth();
        avFrame->linesize[1] = videoFrame->getWidth() / 2;
        avFrame->linesize[2] = videoFrame->getWidth() / 2;
        avFrame->width = videoFrame->getWidth();
        avFrame->height = videoFrame->getHeight();
        avFrame->format = HwAbsMediaFrame::convertVideoFrameFormat(frame->getFormat());
//        avFrame->pts = frameCount;
        avFrame->pts = av_rescale_q_rnd(frame->getPts(),
                                        {1, AV_TIME_BASE},
                                        pVideoStream->time_base,
                                        AV_ROUND_NEAR_INF);

        avcodec_send_frame(ctx, avFrame);
    }
    if (stream) {
        int ret = avcodec_receive_packet(ctx, avPacket);
        if (ret < 0) {
            Logcat::e("HWVC", "HwFFmpegEncoder::encode failed %d: %s",
                      stream->index, strerror(AVUNERROR(ret)));
            return Hw::FAILED;
        }
        firstVideoFrameWrite = true;
        avPacket->stream_index = stream->index;
        avPacket->duration = duration;
        ret = av_write_frame(pFormatCtx, avPacket);
        if (0 == ret) {
            av_packet_unref(avPacket);
            return Hw::SUCCESS;
        }
    }
    av_packet_unref(avPacket);
    return Hw::FAILED;
}

bool HwFFmpegEncoder::stop() {
//    int ret;
//    int got_picture;
//    if (!(pVideoStream->codec->codec->capabilities & AV_CODEC_CAP_DELAY)) {
//        return true;
//    }
//    while (1) {
//        avPacket->data = NULL;
//        avPacket->size = 0;
//        av_init_packet(avPacket);
//        ret = avcodec_encode_video2(pVideoStream->codec, avPacket,
//                                    NULL, &got_picture);
//        av_packet_unref(avPacket);
//        if (ret < 0)
//            break;
//        if (!got_picture) {
//            break;
//        }
//        ret = av_write_frame(pFormatCtx, avPacket);
//        if (ret < 0)
//            break;
//    }
    return true;
}

void HwFFmpegEncoder::release() {
    if (pFormatCtx) {
        av_write_trailer(pFormatCtx);
    }
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
    if (avAudioFrame) {
        av_frame_free(&avAudioFrame);
        avAudioFrame = nullptr;
    }
    if (avFrame) {
        av_frame_free(&avFrame);
        avFrame = nullptr;
    }
    if (aCtx) {
        avcodec_close(aCtx);
        avcodec_free_context(&aCtx);
    }
    if (vCtx) {
        avcodec_close(vCtx);
        avcodec_free_context(&vCtx);
    }
    if (pFormatCtx) {
        if (!(pFormatCtx->flags & AVFMT_NOFILE)) {
            avio_closep(&pFormatCtx->pb);
        }
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
    pAudioStream = nullptr;
    pVideoStream = nullptr;
    if (translator) {
        delete translator;
        translator = nullptr;
    }
}