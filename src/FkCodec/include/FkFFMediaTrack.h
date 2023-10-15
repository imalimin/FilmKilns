//
// Created by lmy on 2023/9/27.
//
#pragma once

#include "FkMediaTrack.h"
#include "FkCodecDefinition.h"
#include "FkPacket.h"
#include "FkAbsFrame.h"
#include "FkFFMediaContext.h"

FK_SUPER_CLASS(FkFFMediaTrack, FkMediaTrack) {
FK_DEF_CLASS_TYPE_FUNC(FkFFMediaTrack)
public:
    FkFFMediaTrack(const std::shared_ptr<FkFFMediaContext> &context, int trackId);

    FkFFMediaTrack(const FkFFMediaTrack &o) = delete;

    virtual ~FkFFMediaTrack();

    /// Grab a decoded frame
    /// \param any std::shared_ptr<FkPacket>
    /// \return std::shared_ptr<FkAbsFrame>
    virtual std::any grab(const std::any &any) override;

private:
    void _destroy();

private:
    int trackId = -1;
    std::weak_ptr<FkFFMediaContext> context;
    AVCodecContext *codecContext = nullptr;
    AVFrame *avFrame = nullptr;
};
