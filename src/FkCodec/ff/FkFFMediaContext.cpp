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

std::shared_ptr<FkPacket> FkFFMediaContext::grab(int trackId) {
    auto itr = cnfGrabStatus.find(trackId);
    if (itr == cnfGrabStatus.end()) {
        cnfGrabStatus[trackId] = true;
        auto src = FkBuffer::wrap(ctx->streams[trackId]->codecpar->extradata, ctx->streams[trackId]->codecpar->extradata_size);
        auto out = FkFFUtils::transAvcExtraData2AnneXB(src);
        FkLogI(TAG, "[%d, %d, %d, %d]", out->data()[0], out->data()[1], out->data()[2], out->data()[3]);
        return FkPacket::wrap(out, 0, 0, FK_CTL_FLAG_CONFIG);
    }
    av_packet_unref(avPacket);
    while (true) {
        auto ret = av_read_frame(ctx, avPacket);
        if (0 != ret) {
            FkLogW(TAG, "Read fail: %x(%s)", ret, av_err2str(ret));
            return nullptr;
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
