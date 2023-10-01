//
// Created by lmy on 2023/9/27.
//
#pragma once

#include "FkObject.h"
#include "FkFFUtils.h"
#include "FkFFMediaTrack.h"
#include "FkPacket.h"
#include <map>

FK_SUPER_CLASS(FkFFMediaContext, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkFFMediaContext)

public:
    FkFFMediaContext(const std::string &path);

    FkFFMediaContext(const FkFFMediaContext &o) = delete;

    virtual ~FkFFMediaContext();

    std::vector<FkFFMediaTrack::Desc> getTracks();

    std::shared_ptr<FkFFMediaTrack> openTrack(int trackId);

    std::shared_ptr<FkPacket> grab(int trackId);

    FkResult seek(int64_t timeInUS);

private:
    void _destroy();

private:
    AVFormatContext *ctx = nullptr;
    AVBSFContext *bsfCtx = nullptr;
    std::string path;
    AVPacket *avPacket = nullptr;
    std::map<int32_t, bool> cnfGrabStatus;
};

