/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include <libyuv.h>
#include "HwAndroidDecoder.h"
#include "BinaryUtils.h"
#include "Logcat.h"
#include "../../../include/HwVideoFrame.h"

HwAndroidDecoder::HwAndroidDecoder() : AbsAudioDecoder(), AbsVideoDecoder() {
}

HwAndroidDecoder::~HwAndroidDecoder() {
    if (avPacket) {
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
    if (bsfPacket) {
        av_packet_free(&bsfPacket);
        bsfPacket = nullptr;
    }
    if (codec) {
//        flush();
        AMediaCodec_stop(codec);
        AMediaCodec_delete(codec);
        codec = nullptr;
    }
    if (aCodecContext) {
        avcodec_close(aCodecContext);
        aCodecContext = nullptr;
    }
    if (vCodecContext) {
        avcodec_close(vCodecContext);
        vCodecContext = nullptr;
    }
    if (pFormatCtx) {
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        pFormatCtx = nullptr;
    }
    if (bsf) {
        av_bsf_free(&bsf);
        bsf = nullptr;
    }
    if (outFrame) {
        delete outFrame;
        outFrame = nullptr;
    }
}

bool HwAndroidDecoder::prepare(string path) {
    this->path = path;
    av_register_all();
    pFormatCtx = avformat_alloc_context();
    //打开输入视频文件
    if (avformat_open_input(&pFormatCtx, path.c_str(), NULL, NULL) != 0) {
        Logcat::e("hwvc", "Couldn't open input stream.");
        return false;
    }
    //获取视频文件信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        Logcat::e("hwvc", "Couldn't find stream information.");
        return false;
    }
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (-1 == videoTrack &&
            AVMediaType::AVMEDIA_TYPE_VIDEO == pFormatCtx->streams[i]->codecpar->codec_type) {
            videoTrack = i;
        }
        if (-1 == audioTrack &&
            AVMediaType::AVMEDIA_TYPE_AUDIO == pFormatCtx->streams[i]->codecpar->codec_type) {
            audioTrack = i;
        }
    }
    if (videoTrack < 0) {
        Logcat::e("HWVC", "HwAndroidDecoder::prepare open video track failed.");
        return false;
    }
    if (!openTrack(videoTrack, &vCodecContext)) {
        Logcat::e("hwvc", "******** Open video track failed. *********");
        return false;
    }
    if (-1 != audioTrack && !openTrack(audioTrack, &aCodecContext)) {
        Logcat::i("HWVC", "******** Open audio track failed. *********");
    }
    Logcat::i("hwvc",
              "HwAndroidDecoder::prepare(%d x %d, du=%lld/%lld channels=%d, sampleHz=%d, frameSize=%d)",
              width(), height(), getVideoDuration(), getAudioDuration(),
              getChannels(), getSampleHz(), aCodecContext ? aCodecContext->frame_size : 0);
    if (aCodecContext) {
        outSampleFormat = av_get_packed_sample_fmt(aCodecContext->sample_fmt);
        if (AV_SAMPLE_FMT_FLT == outSampleFormat || AV_SAMPLE_FMT_DBL == outSampleFormat) {
            outSampleFormat = AV_SAMPLE_FMT_S32;
        }
    }
    avPacket = av_packet_alloc();
    bsfPacket = av_packet_alloc();
    return configureBSF() && configure();
}

bool HwAndroidDecoder::configureBSF() {
    const AVBitStreamFilter *filter = av_bsf_get_by_name("h264_mp4toannexb");
    if (!filter) {
        Logcat::e("HWVC", "HwAndroidDecoder::configureBSF failed");
        return false;
    }
    av_bsf_alloc(filter, &bsf);
    if (!bsf) {
        Logcat::e("HWVC", "HwAndroidDecoder::configureBSF failed");
        return false;
    }
    int ret = avcodec_parameters_from_context(bsf->par_in, vCodecContext);
    if (ret < 0) {
        Logcat::e("HWVC", "HwAndroidDecoder::configureBSF failed");
        return false;
    }
    ret = av_bsf_init(bsf);
    int32_t spsPos = -1, ppsPos = -1;
    const uint8_t *extradata = bsf->par_out->extradata;
    for (int i = 0; i < bsf->par_out->extradata_size; ++i) {
        if (BinaryUtils::match(&extradata[i], {0x00, 0x00, 0x00, 0x01})) {
            if (spsPos < 0) {
                spsPos = i;
                continue;
            }
            if (ppsPos < 0) {
                ppsPos = i;
                break;
            }
        }
    }
    if (spsPos < 0 || ppsPos < 0) {
        Logcat::e("HWVC", "HwAndroidDecoder::configureBSF could not find sps & pps!");
        return false;
    }
    buffers[0] = HwBuffer::alloc(ppsPos - spsPos);
    buffers[1] = HwBuffer::alloc(bsf->par_out->extradata_size - ppsPos);
    memcpy(buffers[0]->getData(), bsf->par_out->extradata + spsPos, buffers[0]->size());
    memcpy(buffers[1]->getData(), bsf->par_out->extradata + ppsPos, buffers[1]->size());
//    FILE *fp = fopen("/sdcard/extra-bsf.data", "wb");
//    fwrite(bsf->par_out->extradata, 1, bsf->par_out->extradata_size, fp);
//    fclose(fp);
    return true;
}

bool HwAndroidDecoder::configure() {
    FILE *fp = fopen("/sdcard/extra.data", "wb");
    fwrite(vCodecContext->extradata, 1, vCodecContext->extradata_size, fp);
    fclose(fp);
    int32_t width = pFormatCtx->streams[videoTrack]->codecpar->width;
    int32_t height = pFormatCtx->streams[videoTrack]->codecpar->height;
    int32_t bitRate = pFormatCtx->streams[videoTrack]->codecpar->bit_rate;
    AMediaFormat *cf = AMediaFormat_new();
    if (AV_CODEC_ID_H264 == pFormatCtx->streams[videoTrack]->codecpar->codec_id) {
        AMediaFormat_setString(cf, AMEDIAFORMAT_KEY_MIME, "video/avc");
        AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_WIDTH, width);
        AMediaFormat_setInt32(cf, AMEDIAFORMAT_KEY_HEIGHT, height);
        if (buffers[0]) {
            AMediaFormat_setBuffer(cf, "csd-0", buffers[0]->getData(), buffers[0]->size());
            if (buffers[1]) {
                AMediaFormat_setBuffer(cf, "csd-1", buffers[1]->getData(), buffers[1]->size());
            }
        }
    }
    const char *mime = nullptr;
    AMediaFormat_getString(cf, AMEDIAFORMAT_KEY_MIME, &mime);
    codec = AMediaCodec_createDecoderByType(mime);
    if (!codec) {
        Logcat::e("HWVC", "HwAndroidDecoder::configure codec alloc failed");
        AMediaFormat_delete(cf);
        return false;
    }
    media_status_t ret = AMediaCodec_configure(codec, cf, nullptr, nullptr, 0);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidDecoder::configure codec configure failed(%d)", ret);
        AMediaFormat_delete(cf);
        return false;
    }
    ret = AMediaCodec_start(codec);
    if (media_status_t::AMEDIA_OK != ret) {
        Logcat::e("HWVC", "HwAndroidDecoder::configure codec start failed(%d)", ret);
        AMediaFormat_delete(cf);
        return false;
    }
    AMediaFormat_delete(cf);
    return true;
}

bool HwAndroidDecoder::openTrack(int track, AVCodecContext **context) {
    AVCodecParameters *avCodecParameters = pFormatCtx->streams[track]->codecpar;
    if (videoTrack == track) {
        Logcat::i("hwvc", "HwAndroidDecoder(%s) %d x %d", path.c_str(), avCodecParameters->width,
                  avCodecParameters->height);
    }
    AVCodec *codec = NULL;
    if (AV_CODEC_ID_H264 == avCodecParameters->codec_id) {
        codec = avcodec_find_decoder(avCodecParameters->codec_id);
//        codec = avcodec_find_decoder_by_name("h264_mediacodec");
//        if (NULL == codec) {
//            LOGE("Selected AV_CODEC_ID_H264.");
//            codec = avcodec_find_decoder(avCodecParameters->codec_id);
//        }
    } else {
        codec = avcodec_find_decoder(avCodecParameters->codec_id);
    }
    if (NULL == codec) {
        Logcat::e("hwvc", "Couldn't find codec.");
        return false;
    }
    //打开编码器
    *context = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(*context, avCodecParameters);
    if (avcodec_open2(*context, codec, NULL) < 0) {
        Logcat::e("hwvc", "Couldn't open codec.");
        return false;
    }
    string typeName = "unknown";
    if (AVMEDIA_TYPE_VIDEO == codec->type) {
        typeName = "video";
    } else if (AVMEDIA_TYPE_AUDIO == codec->type) {
        typeName = "audio";
    }
    Logcat::i("HWVC", "Open %s track with %s, fmt=%d, frameSize=%d", typeName.c_str(), codec->name,
              avCodecParameters->format, avCodecParameters->frame_size);
    return true;
}

HwResult HwAndroidDecoder::push(AVPacket *pkt) {
    ssize_t bufIdx = AMediaCodec_dequeueInputBuffer(codec, 2000);
    if (bufIdx >= 0) {
        if (!pkt) {
            media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx, 0, 0, 0,
                                                                 AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
            if (media_status_t::AMEDIA_OK != result) {
                Logcat::e("HWVC", "HwAndroidDecoder::push queue eof buffer failed(%d)", result);
            }
            return Hw::SUCCESS;
        }
        size_t bufSize = 0;
        int64_t pts = pkt->pts;
        auto buf = AMediaCodec_getInputBuffer(codec, bufIdx, &bufSize);
        if (bufSize > 0) {
            memcpy(buf, pkt->data, pkt->size);
        }
        media_status_t result = AMediaCodec_queueInputBuffer(codec, bufIdx,
                                                             0, bufSize,
                                                             pts, 0);
        if (media_status_t::AMEDIA_OK != result) {
            Logcat::e("HWVC", "HwAndroidDecoder::push queue input buffer failed(%d)", result);
        }
        return Hw::SUCCESS;
    }
    Logcat::e("HWVC", "HwAndroidDecoder::push failed bufIdx = %d", bufIdx);
    return Hw::FAILED;
}

HwResult HwAndroidDecoder::pop(int32_t waitInUS) {
    AMediaCodecBufferInfo info;
    ssize_t bufIdx = AMediaCodec_dequeueOutputBuffer(codec, &info, waitInUS);
    switch (bufIdx) {
        case AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED: {
            Logcat::i("HWVC", "HwAndroidDecoder AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED");
            break;
        }
        case AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED: {
            Logcat::i("HWVC", "HwAndroidDecoder AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED");
            auto *format = AMediaCodec_getOutputFormat(codec);
            AMediaFormat_getInt32(format, "stride", &stride);
            int32_t color = 0;
            AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, &color);
            HwFrameFormat colorFmt;
            if (COLOR_FormatYUV420SemiPlanar == color) {
                colorFmt = HwFrameFormat::HW_IMAGE_NV12;
            } else {
                colorFmt = HwFrameFormat::HW_IMAGE_YV12;
            }
            outFrame = new HwVideoFrame(nullptr, colorFmt, width(), height());
//            uint8_t *sps = nullptr;
//            size_t spsSize = 0;
//            uint8_t *pps = nullptr;
//            size_t ppsSize = 0;
//            AMediaFormat_getBuffer(format, "csd-0", reinterpret_cast<void **>(&sps), &spsSize);
//            AMediaFormat_getBuffer(format, "csd-1", reinterpret_cast<void **>(&pps), &ppsSize);
//            buffers[0] = HwBuffer::alloc(spsSize);
//            buffers[1] = HwBuffer::alloc(ppsSize);
//            memcpy(buffers[0]->getData(), sps, spsSize);
//            memcpy(buffers[1]->getData(), pps, ppsSize);
            break;
        }
        case AMEDIACODEC_INFO_TRY_AGAIN_LATER: {
            Logcat::i("HWVC", "HwAndroidDecoder AMEDIACODEC_INFO_TRY_AGAIN_LATER");
            return Hw::MEDIA_WAIT;
        }
        case AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM: {
            Logcat::i("HWVC", "HwAndroidDecoder AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM");
            return Hw::MEDIA_EOF;
        }
        default: {
            if (bufIdx < 0) break;
            size_t bufSize = 0;
            auto buf = AMediaCodec_getOutputBuffer(codec, bufIdx, &bufSize);
            if (buf) {
                bool wrote = false;
                auto endOfStream = info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM;
                if (endOfStream == 0) {//如果没有收到BUFFER_FLAG_END_OF_STREAM信号，则代表输出数据时有效的
                    if (info.size > 0) {
                        if (info.flags & BUFFER_FLAG_CODEC_CONFIG) {//  config
//                            if (configBuf && configBuf->size() != info.size) {
//                                delete configBuf;
//                                configBuf = nullptr;
//                            }
//                            if (!configBuf) {
//                                configBuf = HwBuffer::alloc(info.size);
//                            }
//                            memcpy(configBuf->getData(), buf, configBuf->size());
                            wrote = false;
                        } else {
                            HwVideoFrame *videoFrame = dynamic_cast<HwVideoFrame *>(outFrame);
                            videoFrame->setPts(info.presentationTimeUs);
                            int32_t w = videoFrame->getWidth();
                            int32_t h = videoFrame->getHeight();
                            if (HwFrameFormat::HW_IMAGE_NV12 == videoFrame->getFormat()) {
                                int pixelCount = w * h;
                                libyuv::NV12ToI420(buf, stride,
                                                   buf + stride * h, stride,
                                                   videoFrame->getBuffer()->getData(),
                                                   videoFrame->getWidth(),
                                                   videoFrame->getBuffer()->getData() + pixelCount,
                                                   videoFrame->getWidth() / 2,
                                                   videoFrame->getBuffer()->getData() +
                                                   pixelCount * 5 / 4,
                                                   videoFrame->getWidth() / 2,
                                                   videoFrame->getWidth(), videoFrame->getHeight());
                            } else {
                                int32_t lines = h * 3 / 2;
                                for (int i = 0; i < lines; ++i) {
                                    memcpy(videoFrame->getBuffer()->getData() + i * w,
                                           buf + info.offset + i * stride * 3 / 2, w);
                                }
                            }
                            wrote = true;
                        }
                    }
                }
                //缓冲区使用完后必须把它还给MediaCodec，以便再次使用，至此一个流程结束，再次循环
                AMediaCodec_releaseOutputBuffer(codec, bufIdx, false);
                return wrote ? Hw::SUCCESS : Hw::FAILED;
            }
        }
    }
    return Hw::FAILED;
}

HwResult HwAndroidDecoder::grab(HwAbsMediaFrame **frame) {
    while (true) {
        int ret = av_read_frame(pFormatCtx, avPacket);
        if (0 == ret) {
            if (videoTrack == avPacket->stream_index) {
                if (bsf) {
                    ret = av_bsf_send_packet(bsf, avPacket);
                    if (0 != ret) {
                        Logcat::i("HWVC", "HwAndroidDecoder::grab av_bsf_send_packet failed");
                    }
                    ret = av_bsf_receive_packet(bsf, bsfPacket);
                    if (0 == ret && bsfPacket) {
                        push(bsfPacket);
                        av_packet_unref(bsfPacket);
                    }
                }

            } else if (audioTrack == avPacket->stream_index) {
//                avcodec_send_packet(aCodecContext, avPacket);
            }
        }
        av_packet_unref(avPacket);//Or av_free_packet?
        if (AVERROR_EOF == ret) {
            eof = true;
        }
        if (Hw::SUCCESS == pop(2000)) {
            outFrame->setPts(av_rescale_q_rnd(outFrame->getPts(),
                                              pFormatCtx->streams[videoTrack]->time_base,
                                              AV_TIME_BASE_Q,
                                              AV_ROUND_NEAR_INF));
            *frame = outFrame;
            return Hw::MEDIA_SUCCESS;
        }
        //如果缓冲区中既没有音频也没有视频，并且已经读取完文件，则播放完了
        if (eof) {
            Logcat::i("HWVC", "HwAndroidDecoder::grab end");
            return Hw::MEDIA_EOF;
        }
    }
}

int HwAndroidDecoder::width() {
    if (!pFormatCtx || videoTrack < 0) return 0;
    return pFormatCtx->streams[videoTrack]->codecpar->width;
}

int HwAndroidDecoder::height() {
    if (!pFormatCtx || videoTrack < 0) return 0;
    return pFormatCtx->streams[videoTrack]->codecpar->height;
}

int HwAndroidDecoder::getChannels() {
    if (!pFormatCtx || audioTrack < 0) return 0;
    return pFormatCtx->streams[videoTrack]->codecpar->channels;
}

int HwAndroidDecoder::getSampleHz() {
    if (!pFormatCtx || audioTrack < 0) return 0;
    return pFormatCtx->streams[audioTrack]->codecpar->sample_rate;
}

int HwAndroidDecoder::getSampleFormat() {
    return outSampleFormat;
}

int HwAndroidDecoder::getSamplesPerBuffer() {
    if (!pFormatCtx || audioTrack < 0) return 0;
    return pFormatCtx->streams[audioTrack]->codecpar->frame_size;
}

void HwAndroidDecoder::seek(int64_t us) {

}

int64_t HwAndroidDecoder::getVideoDuration() {
    if (videoDurationUs >= 0) {
        return videoDurationUs;
    }
    videoDurationUs = pFormatCtx->streams[videoTrack]->duration;
    videoDurationUs = av_rescale_q_rnd(videoDurationUs,
                                       pFormatCtx->streams[videoTrack]->time_base,
                                       AV_TIME_BASE_Q,
                                       AV_ROUND_NEAR_INF);
    if (videoDurationUs < 0) {
        videoDurationUs = pFormatCtx->duration;
    }
    return videoDurationUs;
}

int64_t HwAndroidDecoder::getAudioDuration() {
    if (audioTrack < 0) {
        audioDurationUs = 0;
    }
    if (audioDurationUs >= 0) {
        return audioDurationUs;
    }
    audioDurationUs = pFormatCtx->streams[audioTrack]->duration;
    audioDurationUs = av_rescale_q_rnd(audioDurationUs,
                                       pFormatCtx->streams[audioTrack]->time_base,
                                       AV_TIME_BASE_Q,
                                       AV_ROUND_NEAR_INF);
    if (audioDurationUs < 0) {
        audioDurationUs = pFormatCtx->duration;
    }
    return audioDurationUs;
}

int64_t HwAndroidDecoder::getDuration() {
    if (durationUs >= 0) {
        return durationUs;
    }
    durationUs = pFormatCtx->duration;
    if (durationUs <= 0) {
        durationUs = getAudioDuration();
    }
    if (durationUs <= 0) {
        durationUs = getVideoDuration();
    }
    return durationUs;
}

void HwAndroidDecoder::start() {

}

void HwAndroidDecoder::pause() {

}

void HwAndroidDecoder::stop() {

}
