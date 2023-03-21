/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwPacket.h"

HwPacket *HwPacket::wrap(AVPacket *pkt) {
    HwPacket *p = new HwPacket();
    p->buf = HwBuffer::wrap(pkt->data, pkt->size);
    p->pts = pkt->pts;
    p->dts = pkt->dts;
    p->duration = pkt->duration;
    p->flags = pkt->flags;
    return p;
}

HwPacket *HwPacket::wrap(uint8_t *buf, size_t size, int64_t pts, int64_t dts, int32_t flags) {
    HwPacket *p = new HwPacket();
    if (buf && size > 0) {
        p->buf = HwBuffer::wrap(buf, size);
    }
    p->pts = pts;
    p->dts = dts;
    p->duration = 1;
    p->flags = flags;
    return p;
}

HwPacket *HwPacket::create(size_t size, int64_t pts, int64_t dts) {
    HwPacket *p = new HwPacket();
    p->buf = HwBuffer::alloc(size);
    p->pts = pts;
    p->dts = dts;
    p->duration = 1;
    p->flags = 0;
    return p;
}

HwPacket::HwPacket() : Object() {

}

HwPacket::~HwPacket() {
    if (buf) {
        delete buf;
        buf = nullptr;
    }
    pts = INT64_MIN;
    dts = INT64_MIN;
    duration = INT64_MIN;
    flags = 1;
}

bool HwPacket::ref(AVPacket **pkt) {
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

uint8_t *HwPacket::data() { return buf->data(); }

size_t HwPacket::size() { return buf->size(); }

int64_t HwPacket::getPts() { return pts; }

int64_t HwPacket::getDts() { return dts; }

int64_t HwPacket::getDuration() { return duration; }

void HwPacket::setDuration(int64_t duration) { this->duration = duration; }

int32_t HwPacket::getFlags() {
    return flags;
}

std::string HwPacket::getFlagsStr() {
    std::string str;
    if(flags & AlMediaDef::FLAG_CONFIG) {
        str.append("C");
    } else {
        str.append("_");
    }
    if(flags & AlMediaDef::FLAG_DISPOSABLE) {
        str.append("D");
    } else {
        str.append("_");
    }
    if(flags & AlMediaDef::FLAG_TRUSTED) {
        str.append("T");
    } else {
        str.append("_");
    }
    if(flags & AlMediaDef::FLAG_DISCARD) {
        str.append("D");
    } else {
        str.append("_");
    }
    if(flags & AlMediaDef::FLAG_CORRUPT) {
        str.append("T");
    } else {
        str.append("_");
    }
    if(flags & AlMediaDef::FLAG_KEY) {
        str.append("K");
    } else {
        str.append("_");
    }
    return str;
}
