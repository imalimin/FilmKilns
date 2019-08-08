/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAndroidDecoder.h"
#include "Logcat.h"

HwAndroidDecoder::HwAndroidDecoder() : AbsAudioDecoder(), AbsVideoDecoder() {
    hwFrameAllocator = new HwFrameAllocator();
}

HwAndroidDecoder::~HwAndroidDecoder() {
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
    if (hwFrameAllocator) {
        delete hwFrameAllocator;
        hwFrameAllocator = nullptr;
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
    Logcat::i("hwvc", "HwAndroidDecoder::prepare(%d x %d, du=%lld/%lld channels=%d, sampleHz=%d, frameSize=%d)",
         width(), height(), getVideoDuration(), getAudioDuration(),
         getChannels(), getSampleHz(), aCodecContext ? aCodecContext->frame_size : 0);
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

int HwAndroidDecoder::width() {
    return 0;
}

int HwAndroidDecoder::height() {
    return 0;
}

int HwAndroidDecoder::getChannels() {
    return 0;
}

int HwAndroidDecoder::getSampleHz() {
    return 0;
}

int HwAndroidDecoder::getSampleFormat() {
    return 0;
}

int HwAndroidDecoder::getSamplesPerBuffer() {
    return 0;
}

void HwAndroidDecoder::seek(int64_t us) {

}

HwResult HwAndroidDecoder::grab(HwAbsMediaFrame **frame) {
    return HwResult(0);
}

int64_t HwAndroidDecoder::getVideoDuration() {
    return 0;
}

int64_t HwAndroidDecoder::getAudioDuration() {
    return 0;
}

int64_t HwAndroidDecoder::getDuration() {
    return 0;
}

void HwAndroidDecoder::start() {

}

void HwAndroidDecoder::pause() {

}

void HwAndroidDecoder::stop() {

}
