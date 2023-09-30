//
// Created by lmy on 2023/9/27.
//
#pragma once

#include "FkObject.h"
#include "FkFFUtils.h"
#include "FkFFMediaTrack.h"
#include "FkPacket.h"

FK_SUPER_CLASS(FkFFMediaContext, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkFFMediaContext)

public:
    FkFFMediaContext(const std::string &path);

    FkFFMediaContext(const FkFFMediaContext &o) = delete;

    virtual ~FkFFMediaContext();

    std::vector<FkFFMediaTrack::Desc> getTracks();

    std::shared_ptr<FkFFMediaTrack> openTrack(int trackId);

    std::shared_ptr<FkPacket> grab(int trackId);

private:
    void _destroy();

private:
    AVFormatContext *ctx = nullptr;
    std::string path;
};

