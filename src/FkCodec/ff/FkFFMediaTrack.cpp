//
// Created by lmy on 2023/9/27.
//

#include "FkFFMediaTrack.h"

#define TAG "FkFFMediaTrack"

FK_IMPL_CLASS_TYPE(FkFFMediaTrack, FkObject)

FkFFMediaTrack::FkFFMediaTrack(AVStream *streams) : FkObject() {
    AVCodecParameters *avCodecParameters = streams->codecpar;
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
        FkLogE(TAG, "Couldn't find codec.");
        return;
    }
    //打开编码器
    ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(ctx, avCodecParameters);
    if (avcodec_open2(ctx, codec, NULL) < 0) {
        FkLogE(TAG, "Couldn't open codec.");
        _destroy();
        return;
    }
    std::string typeName = "unknown";
    if (AVMEDIA_TYPE_VIDEO == codec->type) {
        typeName = "video";
    } else if (AVMEDIA_TYPE_AUDIO == codec->type) {
        typeName = "audio";
    }
    FkLogI(TAG, "Open %s track with %s, fmt=%d, frameSize=%d", typeName.c_str(), codec->name,
              avCodecParameters->format, avCodecParameters->frame_size);
}

FkFFMediaTrack::~FkFFMediaTrack() {
    _destroy();
}

void FkFFMediaTrack::_destroy() {
    if (ctx) {
        avcodec_close(ctx);
        ctx = nullptr;
    }
}

std::shared_ptr<FkAbsFrame> FkFFMediaTrack::decode(const std::shared_ptr<FkPacket> &pkt) {
    return nullptr;
}
