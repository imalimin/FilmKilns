/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwPacket.h"

HwPacket *HwPacket::wrap(AVPacket *pkt) {
    HwPacket *p = new HwPacket();
    p->data = HwBuffer::wrap(pkt->data, pkt->size);
    p->pts = pkt->pts;
    p->dts = pkt->dts;
    p->duration = pkt->duration;
    p->flags = pkt->flags;
    return p;
}

HwPacket *HwPacket::wrap(uint8_t *buf, size_t size, int64_t pts, int64_t dts) {
    HwPacket *p = new HwPacket();
    p->data = HwBuffer::wrap(buf, size);
    p->pts = pts;
    p->dts = dts;
    p->duration = 1;
    p->flags = 1;
    return p;
}

HwPacket *HwPacket::create(size_t size, int64_t pts, int64_t dts) {
    HwPacket *p = new HwPacket();
    p->data = HwBuffer::alloc(size);
    p->pts = pts;
    p->dts = dts;
    p->duration = 1;
    p->flags = 1;
    return p;
}

HwPacket::HwPacket() : Object() {

}

HwPacket::~HwPacket() {
    if (data) {
        delete data;
        data = nullptr;
    }
    pts = INT64_MIN;
    dts = INT64_MIN;
}

bool HwPacket::ref(AVPacket **pkt) {
    if (!pkt || !*pkt) {
        return false;
    }
    (*pkt)->data = buf();
    (*pkt)->size = size();
    (*pkt)->pts = getPts();
    (*pkt)->dts = getDts();
    (*pkt)->duration = getDuration();
    (*pkt)->flags = flags;
    return true;
}

uint8_t *HwPacket::buf() { return data->getData(); }

size_t HwPacket::size() { return data->size(); }

int64_t HwPacket::getPts() { return pts; }

int64_t HwPacket::getDts() { return dts; }

int64_t HwPacket::getDuration() { return duration; }

void HwPacket::setDuration(int64_t duration) { this->duration = duration; }