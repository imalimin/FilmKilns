/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwFrameAllocator.h"
#include "../include/HwVideoFrame.h"
#include "../include/HwAudioFrame.h"

HwFrameAllocator::HwFrameAllocator() : HwSourcesAllocator() {

}

HwFrameAllocator::~HwFrameAllocator() {
    unRefLock.lock();
    set<HwAbsMediaFrame *>::iterator itr = unRefQueue.begin();
    while (itr != unRefQueue.end()) {
        HwAbsMediaFrame *frame = *itr;
        delete frame;
        ++itr;
    }
    unRefQueue.clear();
    unRefLock.unlock();
    refLock.lock();
    itr = refQueue.begin();
    while (itr != refQueue.end()) {
        HwAbsMediaFrame *frame = *itr;
        delete frame;
        ++itr;
    }
    refQueue.clear();
    refLock.unlock();
}

HwAbsMediaFrame *HwFrameAllocator::ref(AVFrame *avFrame) {
    if (!avFrame) return nullptr;
    if (avFrame->width * avFrame->height > 0) {
        return refVideo(avFrame);
    }
    return refAudio(avFrame);
}

bool HwFrameAllocator::recycle(HwSources **entity) {
    HwAbsMediaFrame *frame = reinterpret_cast<HwAbsMediaFrame *>(entity[0]);
//    entity[0] = nullptr;
    refLock.lock();
    set<HwAbsMediaFrame *>::iterator itr = refQueue.find(frame);
    if(refQueue.end() == itr){
        Logcat::i("HWVC", "HwSources(%p) recycle by allocator(%p) failed a", frame, this);
        refLock.unlock();
        return false;
    }
    set<HwAbsMediaFrame *>::iterator itr2 = refQueue.erase(itr);
    if(refQueue.end() == itr2){
        Logcat::i("HWVC", "HwSources(%p) recycle by allocator(%p) failed b", frame, this);
        refLock.unlock();
        return false;
    }
    refLock.unlock();
    unRefLock.lock();
    unRefQueue.insert(*itr);
    unRefLock.unlock();
    return true;
}

bool HwFrameAllocator::isRef(HwAbsMediaFrame *frame) {
    return refQueue.end() != refQueue.find(frame);
}

HwAbsMediaFrame *HwFrameAllocator::refAudio(AVFrame *avFrame) {
    HwAbsMediaFrame *frame = nullptr;
    unRefLock.lock();
    if (unRefQueue.size() > 0) {
        set<HwAbsMediaFrame *>::iterator itr = unRefQueue.begin();
        while (itr != unRefQueue.end()) {
            if ((*itr)->isAudio()
                && (*itr)->getBufferSize() == avFrame->linesize[0]) {//帧类型相同，data大小相等，则可以复用
                frame = *itr;
                unRefQueue.erase(itr);
                break;
            }
            ++itr;
        }
    }
    unRefLock.unlock();
    if (!frame) {
        frame = new HwAudioFrame(this,
                                 HwAbsMediaFrame::convertToAudioFrameFormat(
                                         static_cast<AVSampleFormat>(avFrame->format)),
                                 static_cast<uint16_t>(avFrame->channels),
                                 static_cast<uint32_t>(avFrame->sample_rate),
                                 static_cast<uint64_t>(avFrame->nb_samples));
    }
    memset(frame->getBuffer()->getData(), 0, frame->getBufferSize());
    copyInfo(frame, avFrame);
    refLock.lock();
    refQueue.insert(frame);
    refLock.unlock();
    return frame;
}

HwAbsMediaFrame *HwFrameAllocator::refVideo(AVFrame *avFrame) {
    HwAbsMediaFrame *frame = nullptr;
    unRefLock.lock();
    if (unRefQueue.size() > 0) {
        int byteCount = HwAbsMediaFrame::getImageSize(
                HwAbsMediaFrame::convertToVideoFrameFormat(
                        static_cast<AVPixelFormat>(avFrame->format)),
                avFrame->width, avFrame->height);
        set<HwAbsMediaFrame *>::iterator itr = unRefQueue.begin();
        while (itr != unRefQueue.end()) {
            if ((*itr)->isVideo()
                && (*itr)->getBufferSize() == byteCount) {//帧类型相同，data大小相等，则可以复用
                frame = *itr;
                unRefQueue.erase(itr);
                break;
            }
            ++itr;
        }
    }
    unRefLock.unlock();
    if (!frame) {
        frame = new HwVideoFrame(this,
                                 HwAbsMediaFrame::convertToVideoFrameFormat(
                                         static_cast<AVPixelFormat>(avFrame->format)),
                                 static_cast<uint32_t>(avFrame->width),
                                 static_cast<uint32_t>(avFrame->height));
    }
    frame->setPts(avFrame->pts);
    int pixelCount = avFrame->width * avFrame->height;
    switch (frame->getFormat()) {
        case HW_IMAGE_YV12: {
            memcpy(frame->getBuffer()->getData(), avFrame->data[0], pixelCount);
            memcpy(frame->getBuffer()->getData() + pixelCount, avFrame->data[1], pixelCount / 4);
            memcpy(frame->getBuffer()->getData() + pixelCount + pixelCount / 4, avFrame->data[2],
                   pixelCount / 4);
            break;
        }
        case HW_IMAGE_NV12: {
            memcpy(frame->getBuffer()->getData(), avFrame->data[0], pixelCount);
            int uvSize = pixelCount / 4;
            for (int i = 0; i < uvSize; ++i) {
                *(frame->getBuffer()->getData() + pixelCount + i) = avFrame->data[1][i * 2];
                *(frame->getBuffer()->getData() + pixelCount + uvSize + i) = avFrame->data[1][
                        i * 2 + 1];
            }
            break;
        }
        default:
            copyInfo(frame, avFrame);
    }
    refLock.lock();
    refQueue.insert(frame);
    refLock.unlock();
    return frame;
}

void HwFrameAllocator::copyInfo(HwAbsMediaFrame *dest, AVFrame *src) {
    memcpy(dest->getBuffer()->getData(), src->data[0], dest->getBufferSize());
    dest->setPts(src->pts);
}

HwAbsMediaFrame *HwFrameAllocator::ref(HwAbsMediaFrame *src) {
    HwAbsMediaFrame *frame = nullptr;
    unRefLock.lock();
    if (unRefQueue.size() > 0) {
        set<HwAbsMediaFrame *>::iterator itr = unRefQueue.begin();
        while (itr != unRefQueue.end()) {
            if ((*itr)->getFormat() == src->getFormat()
                && (*itr)->getBufferSize() == src->getBufferSize()) {
                frame = *itr;
                set<HwAbsMediaFrame *>::iterator itr2 = unRefQueue.erase(itr);
                break;
            }
            ++itr;
        }
    }
    unRefLock.unlock();
    if (frame) {
        memcpy(frame->getBuffer()->getData(), src->getBuffer()->getData(), frame->getBufferSize());
        frame->setPts(src->getPts());
    } else {
        if (src->isVideo()) {
            HwVideoFrame *videoFrame = static_cast<HwVideoFrame *>(src);

            frame = new HwVideoFrame(this, videoFrame->getFormat(),
                                     videoFrame->getWidth(),
                                     videoFrame->getHeight());
            videoFrame->clone(frame);
            Logcat::i("HWVC", "HwSources(%p) alloc by allocator(%p)", frame, this);
        } else if (src->isAudio()) {
            HwAudioFrame *audioFrame = static_cast<HwAudioFrame *>(src);

            frame = new HwAudioFrame(this, audioFrame->getFormat(), audioFrame->getChannels(),
                                     audioFrame->getSampleRate(), audioFrame->getSampleCount());
            audioFrame->clone(frame);
        }
    }
    refLock.lock();
    refQueue.insert(frame);
    refLock.unlock();
    return frame;
}

void HwFrameAllocator::printInfo() {
    Logcat::i("HWVC", "HwFrameAllocator(%p)::info: ref=%d, unRef=%d",
              this,
              refQueue.size(),
              unRefQueue.size());
}