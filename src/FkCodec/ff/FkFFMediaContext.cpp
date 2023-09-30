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
    return nullptr;
}
