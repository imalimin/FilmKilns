//
// Created by lmy on 2023/10/15.
//

#include "FkFFramePacket.h"

FK_IMPL_CLASS_TYPE(FkFFramePacket, FkObject)

std::shared_ptr<FkFFramePacket> FkFFramePacket::make(AVFrame *frame,
                                                     kMediaType type,
                                                     AVRational avTimeBase) {
    auto result = std::make_shared<FkFFramePacket>();
    result->type = type;
    result->avTimeBase = avTimeBase;
    result->frame = av_frame_alloc();
    av_frame_ref(result->frame, frame);
    return result;
}

FkFFramePacket::FkFFramePacket() : FkAbsPacket() {
    av_frame_unref(this->frame);
    this->frame = nullptr;
}


FkFFramePacket::~FkFFramePacket() {
}

FkTime FkFFramePacket::getPts() const {
    return {this->frame->pts, {avTimeBase.num * AV_TIME_BASE_Q.den, avTimeBase.den  * AV_TIME_BASE_Q.num}};
}

FkTime FkFFramePacket::getDts() const {
    return {this->frame->pkt_dts, {avTimeBase.num * AV_TIME_BASE_Q.den, avTimeBase.den  * AV_TIME_BASE_Q.num}};
}

FkTime FkFFramePacket::getDuration() const {
    return {this->frame->pkt_duration, {avTimeBase.num * AV_TIME_BASE_Q.den, avTimeBase.den  * AV_TIME_BASE_Q.num}};
}

std::shared_ptr<FkBuffer> FkFFramePacket::getBuffer() const {
    return FkBuffer::wrap(this->frame->data[0], this->frame->linesize[0]);
}

int32_t FkFFramePacket::getFlags() const {
    return this->frame->flags;
}

kMediaType FkFFramePacket::getType() const {
    return type;
}

std::shared_ptr<FkAbsPacket> FkFFramePacket::clone() {
    auto _frame = av_frame_clone(this->frame);
    auto result = make(_frame, type, avTimeBase);
    av_frame_unref(_frame);
    return result;
}

void *FkFFramePacket::getOrigin() const {
    return frame;
}