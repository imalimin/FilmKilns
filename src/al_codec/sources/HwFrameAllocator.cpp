/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwFrameAllocator.h"
#include "HwVideoFrame.h"
#include "HwAudioFrame.h"

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
    if (refQueue.end() == itr) {
        Logcat::i("HWVC", "HwSources(%p) recycle by allocator(%p) failed a", frame, this);
        refLock.unlock();
        return false;
    }
    refQueue.erase(itr);
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
        auto avFrameByteSize = avFrame->nb_samples *
                               av_get_bytes_per_sample((AVSampleFormat) avFrame->format) *
                               avFrame->channels;
        set<HwAbsMediaFrame *>::iterator itr = unRefQueue.begin();
        while (itr != unRefQueue.end()) {
            if ((*itr)->isAudio()
                && (*itr)->size() == avFrameByteSize) {//帧类型相同，data大小相等，则可以复用
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
    memset(frame->data(), 0, frame->size());
    copyInfo(frame, avFrame);
    refLock.lock();
    refQueue.insert(frame);
    refLock.unlock();
    return frame;
}

HwAbsMediaFrame *HwFrameAllocator::ref(uint8_t *buf, int nbSample) {
    return nullptr;
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
                && (*itr)->size() == byteCount) {//帧类型相同，data大小相等，则可以复用
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
    frame->setFormat(HwAbsMediaFrame::convertToVideoFrameFormat(
            static_cast<AVPixelFormat>(avFrame->format)));
    frame->setPts(avFrame->pts);
    av_image_copy_to_buffer(frame->data(), frame->size(), (const uint8_t *const *) avFrame->data,
                            avFrame->linesize, (AVPixelFormat) avFrame->format,
                            avFrame->width, avFrame->height, 1);
    refLock.lock();
    refQueue.insert(frame);
    refLock.unlock();
    return frame;
}

void HwFrameAllocator::copyInfo(HwAbsMediaFrame *dest, AVFrame *src) {
    memcpy(dest->data(), src->data[0], dest->size());
    dest->setPts(src->pts);
}

HwAbsMediaFrame *HwFrameAllocator::ref(HwAbsMediaFrame *src) {
    HwAbsMediaFrame *frame = nullptr;
    unRefLock.lock();
    if (unRefQueue.size() > 0) {
        set<HwAbsMediaFrame *>::iterator itr = unRefQueue.begin();
        while (itr != unRefQueue.end()) {
            if ((*itr)->getFormat() == src->getFormat()
                && (*itr)->size() == src->size()) {
                frame = *itr;
                unRefQueue.erase(itr);
                break;
            }
            ++itr;
        }
    }
    unRefLock.unlock();
    if (frame) {
        src->clone(frame);
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