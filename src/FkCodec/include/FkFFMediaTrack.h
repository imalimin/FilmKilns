//
// Created by lmy on 2023/9/27.
//
#pragma once

#include "FkObject.h"
#include "FkCodecDefinition.h"
#include "FkPacket.h"
#include "FkAbsFrame.h"

class FkFFMediaContext;

FK_SUPER_CLASS(FkFFMediaTrack, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkFFMediaTrack)

    friend FkFFMediaContext;
public:
    struct Desc {
        int trackId = -1;
        kMediaType type = kMediaType::NONE;
    };

    FkFFMediaTrack(AVStream *streams);

    FkFFMediaTrack(const FkFFMediaTrack &o) = delete;

    virtual ~FkFFMediaTrack();

    std::shared_ptr<FkAbsFrame> decode(const std::shared_ptr<FkPacket> &pkt);

private:
    void _destroy();

private:
    AVCodecContext *ctx = nullptr;
};
