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
    if (AV_CODEC_ID_NONE != pFormatCtx->oformat->video_codec) {
        openVideoTrack();
    }
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
    AVCodecContext *pCodecCtx = pVideoStream->codec;
    // Configure
    pCodecCtx->codec_id = pFormatCtx->oformat->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = width;
    pCodecCtx->height = height;
    pCodecCtx->bit_rate = width * height * 3;
    pCodecCtx->profile = FF_PROFILE_H264_HIGH;
    pCodecCtx->gop_size = 15;

    pCodecCtx->time_base = pVideoStream->time_base;

    pCodecCtx->thread_count = 0;
    pCodecCtx->qmin = 10;
    pCodecCtx->qmax = 30;
    pCodecCtx->max_b_frames = 2;

    AVDictionary *param = nullptr;
    if (AV_CODEC_ID_H264 == pCodecCtx->codec_id) {
        av_dict_set(&param, "preset", "superfast", 0);
        av_dict_set(&param, "tune", "zerolatency", 0);
        av_dict_set(&param, "crf", "15", 0);
        //av_dict_set(param, "profile", "main", 0)
    }
    int ret = avcodec_open2(pCodecCtx, pCodec, &param);
    av_dict_free(&param);
    if (ret < 0) {
        release();
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not open %d codec!",
                  pCodecCtx->codec_id);
        return false;
    }
    /**
     * Important. If not set this, the output will be fail.
     */
    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        pCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    ret = avcodec_parameters_from_context(pVideoStream->codecpar, pVideoStream->codec);
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
    AVCodecContext *pCodecCtx = pAudioStream->codec;
    pCodecCtx->codec_id = pFormatCtx->oformat->audio_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_AUDIO;
    pCodecCtx->sample_fmt = AV_SAMPLE_FMT_FLTP;
    pCodecCtx->bit_rate = 64000;
    pCodecCtx->sample_rate = this->audioFormat.getSampleRate();
    pCodecCtx->channels = this->audioFormat.getChannels();
    pCodecCtx->channel_layout = static_cast<uint64_t>(
            av_get_default_channel_layout(pCodecCtx->channels));
    pCodecCtx->time_base = {1, pCodecCtx->sample_rate};
    AVDictionary *param = nullptr;
    int ret = avcodec_open2(pCodecCtx, pCodec, &param);
    av_dict_free(&param);
    if (ret < 0) {
        if (pAudioStream && pAudioStream->codec) {
            avcodec_close(pAudioStream->codec);
            pAudioStream->codec = nullptr;
            pAudioStream = nullptr;
        }
        Logcat::e("HWVC", "HwFFmpegEncoder::initialize could not open audio codec: %s",
                  strerror(AVUNERROR(ret)));
        return false;
    }
    if (pFormatCtx->oformat->flags & AVFMT_GLOBALHEADER) {
        pCodecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    ret = avcodec_parameters_from_context(pAudioStream->codecpar, pAudioStream->codec);
    return true;
}

HwResult HwFFmpegEncoder::write(HwAbsMediaFrame *frame) {
    AVStream *stream = nullptr;
    int64_t duration = 1;
    if (frame->isAudio() && pAudioStream) {
        // Ensure that the first frame is video.
        if (!firstVideoFrameWrite) {
            return Hw::FAILED;
        }
        stream = pAudioStream;
        duration = static_cast<int64_t>(1.0 / stream->codec->sample_rate * frame->getBufferSize());
        HwAudioFrame *audioFrame = dynamic_cast<HwAudioFrame *>(frame);
        avAudioFrame->data[0] = audioFrame->getBuffer()->getData();
        avAudioFrame->linesize[0] = audioFrame->getBufferSize();
        avAudioFrame->format = HwAbsMediaFrame::convertAudioFrameFormat(frame->getFormat());
        avAudioFrame->sample_rate = audioFrame->getSampleRate();
        avAudioFrame->nb_samples = 1024;
        avAudioFrame->channels = audioFrame->getChannels();
        avAudioFrame->channel_layout = static_cast<uint64_t>(av_get_default_channel_layout(
                audioFrame->getChannels()));
        avFrame->pts = av_rescale_q_rnd(frame->getPts(),
                                        {1, AV_TIME_BASE},
                                        pAudioStream->time_base,
                                        AV_ROUND_NEAR_INF);

        AVFrame *avFrame = nullptr;
        translator->translate(&avFrame, &avAudioFrame);
        avcodec_send_frame(stream->codec, avFrame);
    } else if (frame->isVideo() && pVideoStream) {
        stream = pVideoStream;
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
        avFrame->pts = av_rescale_q_rnd(frame->getPts(),
                                        {1, AV_TIME_BASE},
                                        pVideoStream->time_base,
                                        AV_ROUND_NEAR_INF);
        avFrame->format = HwAbsMediaFrame::convertVideoFrameFormat(frame->getFormat());

        avcodec_send_frame(stream->codec, avFrame);
    }
    if (stream) {
        int ret = avcodec_receive_packet(stream->codec, avPacket);
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
    if (pAudioStream && pAudioStream->codec) {
        avcodec_close(pAudioStream->codec);
        avcodec_free_context(&pAudioStream->codec);
        pAudioStream = nullptr;
    }
    if (pVideoStream && pVideoStream->codec) {
        avcodec_close(pVideoStream->codec);
        avcodec_free_context(&pVideoStream->codec);
        pVideoStream = nullptr;
    }
    if (pFormatCtx) {
        if (!(pFormatCtx->flags & AVFMT_NOFILE)) {
            avio_closep(&pFormatCtx->pb);
        }
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
    if (translator) {
        delete translator;
        translator = nullptr;
    }
}