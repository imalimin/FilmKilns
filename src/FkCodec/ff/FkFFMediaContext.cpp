//
// Created by lmy on 2023/9/27.
//

#include "FkFFMediaContext.h"
#include "FkFFUtils.h"

#define TAG "FkFFMediaContext"

FK_IMPL_CLASS_TYPE(FkFFMediaContext, FkObject)

FkFFMediaContext::FkFFMediaContext(const std::string &path) : FkObject(), path(path) {
    ctx = avformat_alloc_context();
    if (avformat_open_input(&ctx, path.c_str(), NULL, NULL) != 0) {
        FkLogE(TAG, "Couldn't open input stream.");
        _destroy();
    }
    if (avformat_find_stream_info(ctx, NULL) < 0) {
        FkLogW(TAG, "Couldn't find stream information.");
    }
    avPacket = av_packet_alloc();
}

FkFFMediaContext::~FkFFMediaContext() {
    _destroy();
    if (bsfContext) {
        av_bsf_free(&bsfContext);
        bsfContext = nullptr;
    }
}

void FkFFMediaContext::_destroy() {
    if (ctx) {
        avformat_close_input(&ctx);
        avformat_free_context(ctx);
        ctx = nullptr;
    }
    if (avPacket) {
        av_packet_unref(avPacket);
        av_packet_free(&avPacket);
        avPacket = nullptr;
    }
}

std::vector<FkFFMediaTrack::Desc> FkFFMediaContext::getTracks() {
    std::vector<FkFFMediaTrack::Desc> vec;
    if (ctx == nullptr) {
        return vec;
    }
    for (int i = 0; i < ctx->nb_streams; i++) {
        FkFFMediaTrack::Desc desc{i, FkFFUtils::makeMediaType(ctx->streams[i]->codecpar->codec_type)};
        vec.emplace_back(desc);
    }
    return vec;
}

std::shared_ptr<FkFFMediaTrack> FkFFMediaContext::openTrack(int trackId) {
    return std::make_shared<FkFFMediaTrack>(ctx->streams[trackId]);
}

FkFFMediaTrack::Desc FkFFMediaContext::_getVideoTrack() {
    for (int i = 0; i < ctx->nb_streams; i++) {
        if (ctx->streams[i]->codecpar->codec_type == AVMediaType::AVMEDIA_TYPE_VIDEO) {
            return {i, FkFFUtils::makeMediaType(ctx->streams[i]->codecpar->codec_type)};
        }
    }
    return {-1, kMediaType::NONE};
}

void FkFFMediaContext::_checkBsf(int trackId) {
    if (ctx->streams[trackId]->codecpar->codec_id == AV_CODEC_ID_H264 && bsfContext == nullptr) {
        const AVBitStreamFilter *bsf = av_bsf_get_by_name("h264_mp4toannexb");
        if (bsf != nullptr) {
            if (av_bsf_alloc(bsf, &bsfContext) == 0) {
                auto ret = avcodec_parameters_copy(bsfContext->par_in, ctx->streams[trackId]->codecpar);
                if (ret < 0 || av_bsf_init(bsfContext) < 0) {
                    av_bsf_free(&bsfContext);
                    bsfContext = nullptr;
                }
            }
        }
    }
}

std::shared_ptr<FkPacket> FkFFMediaContext::grab() {
    auto desc = _getVideoTrack();
    if (desc.trackId >= 0) {
        auto trackId = desc.trackId;
        auto itr = cnfGrabStatus.find(trackId);
        if (itr == cnfGrabStatus.end()) {
            cnfGrabStatus[trackId] = true;
            auto src = FkBuffer::wrap(ctx->streams[trackId]->codecpar->extradata, ctx->streams[trackId]->codecpar->extradata_size);
            auto out = FkFFUtils::transAvcExtraData2AnneXB(src);
            FkLogI(TAG, "[%d, %d, %d, %d]", out->data()[0], out->data()[1], out->data()[2], out->data()[3]);
            auto pkt = FkPacket::wrap(out, 0, 0, FK_CTL_FLAG_CONFIG);
            pkt->setType(kMediaType::VIDEO);
            return pkt;
        }
        _checkBsf(trackId);
    }

    kMediaType type = kMediaType::AUDIO;
    av_packet_unref(avPacket);
    while (true) {
        auto ret = av_read_frame(ctx, avPacket);
        if (ret == AVERROR(EOF)) {
            FkLogW(TAG, "Read fail: %x(%s)", ret, av_err2str(ret));
            return nullptr;
        }
        if (avPacket->stream_index == desc.trackId) {
            type = kMediaType::VIDEO;
            ret = av_bsf_send_packet(bsfContext, avPacket);
            ret = av_bsf_receive_packet(bsfContext, avPacket);
            if (ret != 0) {
                continue;
            }
        }
        FkLogI(TAG, "[%d, %d, %d, %d]", avPacket->data[0], avPacket->data[1], avPacket->data[2], avPacket->data[3]);
        break;
    }
    auto pkt = FkPacket::wrap2(avPacket, ctx->streams[avPacket->stream_index]->time_base);
    pkt->setType(type);
    return pkt;
}

std::shared_ptr<FkPacket> FkFFMediaContext::grab(int trackId) {
    auto itr = cnfGrabStatus.find(trackId);
    if (itr == cnfGrabStatus.end()) {
        cnfGrabStatus[trackId] = true;
        auto src = FkBuffer::wrap(ctx->streams[trackId]->codecpar->extradata, ctx->streams[trackId]->codecpar->extradata_size);
        auto out = FkFFUtils::transAvcExtraData2AnneXB(src);
        FkLogI(TAG, "[%d, %d, %d, %d]", out->data()[0], out->data()[1], out->data()[2], out->data()[3]);
        return FkPacket::wrap(out, 0, 0, FK_CTL_FLAG_CONFIG);
    }
    _checkBsf(trackId);

    av_packet_unref(avPacket);
    while (true) {
        auto ret = av_read_frame(ctx, avPacket);
        if (ret == AVERROR(EOF)) {
            FkLogW(TAG, "Read fail: %x(%s)", ret, av_err2str(ret));
            return nullptr;
        }
        ret = av_bsf_send_packet(bsfContext, avPacket);
        ret = av_bsf_receive_packet(bsfContext, avPacket);
        if (ret != 0) {
            continue;
        }

        FkLogI(TAG, "[%d, %d, %d, %d]", avPacket->data[0], avPacket->data[1], avPacket->data[2], avPacket->data[3]);
        if (trackId < 0 || avPacket->stream_index == trackId) {
            break;
        }
    }
    return FkPacket::wrap2(avPacket, ctx->streams[trackId]->time_base);
}

FkResult FkFFMediaContext::seek(int64_t timeInUS) {
    int ret = avformat_seek_file(ctx, -1, INT64_MIN,
                                 timeInUS, INT64_MAX,
                                 AVSEEK_FLAG_BACKWARD);
    return ret >= 0 ? FK_OK : FK_FAIL;
}
