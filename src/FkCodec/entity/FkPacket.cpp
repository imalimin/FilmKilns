/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FkPacket.h"
#include "FkCodecDefinition.h"

FK_IMPL_CLASS_TYPE(FkPacket, FkObject)

FkPacket *FkPacket::wrap(AVPacket *pkt) {
    FkPacket *p = new FkPacket();
    p->buf = FkBuffer::wrap(pkt->data, pkt->size);
    p->pts = pkt->pts;
    p->dts = pkt->dts;
    p->duration = pkt->duration;
    p->flags = pkt->flags;
    return p;
}

FkPacket *FkPacket::wrap(uint8_t *buf, size_t size, int64_t pts, int64_t dts, int32_t flags) {
    FkPacket *p = new FkPacket();
    if (buf && size > 0) {
        p->buf = FkBuffer::wrap(buf, size);
    }
    p->pts = pts;
    p->dts = dts;
    p->duration = 1;
    p->flags = flags;
    return p;
}

FkPacket *FkPacket::create(size_t size, int64_t pts, int64_t dts) {
    FkPacket *p = new FkPacket();
    p->buf = FkBuffer::alloc(size);
    p->pts = pts;
    p->dts = dts;
    p->duration = 1;
    p->flags = 0;
    return p;
}

FkPacket::FkPacket() : FkObject() {

}

FkPacket::~FkPacket() {
    buf = nullptr;
    pts = INT64_MIN;
    dts = INT64_MIN;
    duration = INT64_MIN;
    flags = 1;
}

bool FkPacket::ref(AVPacket **pkt) {
    if (!pkt || !*pkt) {
        return false;
    }
    (*pkt)->data = data();
    (*pkt)->size = size();
    (*pkt)->pts = getPts();
    (*pkt)->dts = getDts();
    (*pkt)->duration = getDuration();
    (*pkt)->flags = flags;
    return true;
}

uint8_t *FkPacket::data() { return buf->data(); }

size_t FkPacket::size() { return buf->size(); }

int64_t FkPacket::getPts() { return pts; }

int64_t FkPacket::getDts() { return dts; }

int64_t FkPacket::getDuration() { return duration; }

void FkPacket::setDuration(int64_t _duration) { this->duration = _duration; }

int32_t FkPacket::getFlags() {
    return flags;
}

std::string FkPacket::getFlagsStr() {
    std::string str;
    if(flags & FK_CTL_FLAG_CONFIG) {
        str.append("C");
    } else {
        str.append("_");
    }
    if(flags & FK_CTL_FLAG_DISPOSABLE) {
        str.append("D");
    } else {
        str.append("_");
    }
    if(flags & FK_CTL_FLAG_TRUSTED) {
        str.append("T");
    } else {
        str.append("_");
    }
    if(flags & FK_CTL_FLAG_DISCARD) {
        str.append("D");
    } else {
        str.append("_");
    }
    if(flags & FK_CTL_FLAG_CORRUPT) {
        str.append("T");
    } else {
        str.append("_");
    }
    if(flags & FK_CTL_FLAG_KEY) {
        str.append("K");
    } else {
        str.append("_");
    }
    return str;
}
