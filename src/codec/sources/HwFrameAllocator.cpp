/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwFrameAllocator.h"
#include "../include/HwVideoFrame.h"
#include "../include/HwAudioFrame.h"
#include <sstream>

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
        set<HwAbsMediaFrame *>::iterator itr = unRefQueue.begin();
        while (itr != unRefQueue.end()) {
            if ((*itr)->isAudio()
                && (*itr)->size() == avFrame->linesize[0]) {//帧类型相同，data大小相等，则可以复用
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
    frame->setPts(avFrame->pts);
    int pixelCount = avFrame->width * avFrame->height;
    switch (frame->getFormat()) {
        case HwFrameFormat::HW_IMAGE_YV12: {
            if (avFrame->linesize[0] == avFrame->width) {
                memcpy(frame->data(), avFrame->data[0], pixelCount);
                memcpy(frame->data() + pixelCount, avFrame->data[1],
                       pixelCount / 4);
                memcpy(frame->data() + pixelCount + pixelCount / 4,
                       avFrame->data[2],
                       pixelCount / 4);
            } else {
                int position = 0;
                for (int i = 0; i < avFrame->height; ++i) {
                    memcpy(frame->data() + position,
                           avFrame->data[0] + i * avFrame->linesize[0], avFrame->width);
                    position += avFrame->width;
                }
                for (int i = 0; i < avFrame->height / 2; ++i) {
                    memcpy(frame->data() + position,
                           avFrame->data[1] + i * avFrame->linesize[1], avFrame->width / 2);
                    position += avFrame->width / 2;
                }
                for (int i = 0; i < avFrame->height / 2; ++i) {
                    memcpy(frame->data() + position,
                           avFrame->data[2] + i * avFrame->linesize[2], avFrame->width / 2);
                    position += avFrame->width / 2;
                }
            }
            break;
        }
        case HwFrameFormat::HW_IMAGE_NV12: {
            memcpy(frame->data(), avFrame->data[0], pixelCount);
            int uvSize = pixelCount / 4;
            for (int i = 0; i < uvSize; ++i) {
                *(frame->data() + pixelCount + i) = avFrame->data[1][i * 2];
                *(frame->data() + pixelCount + uvSize + i) = avFrame->data[1][
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
        memcpy(frame->data(), src->data(), frame->size());
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