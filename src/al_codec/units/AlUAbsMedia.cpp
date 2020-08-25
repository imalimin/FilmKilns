/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUAbsMedia.h"
#include "StringUtils.h"
#include "AlMediaTrack.h"

#define TAG "AlUAbsMedia"

AlUAbsMedia::AlUAbsMedia(const std::string alias, AlMediaTrack::kType type)
        : Unit(alias), type(type) {
}

AlUAbsMedia::~AlUAbsMedia() {
}

bool AlUAbsMedia::onCreate(AlMessage *msg) {
    return true;
}

bool AlUAbsMedia::onDestroy(AlMessage *msg) {
    for (auto &it : map) {
        it.second->stop();
    }
    map.clear();
    mLastFrameMap.clear();
    return true;
}

bool AlUAbsMedia::_onAddTrack(AlMessage *msg) {
    auto clip = std::static_pointer_cast<AlMediaClip>(msg->sp);
    int64_t duration = 0;
    int64_t frameDuration = 0;
    _create(clip.get(), duration, frameDuration);
    if (duration > 0) {
        clip->setDuration(duration);
        clip->setFrameDuration(frameDuration);
        auto msg1 = AlMessage::obtain(MSG_SEQUENCE_TRACK_SET_DURATION);
        msg1->sp = clip;
        postMessage(msg1);
    }
    return true;
}

bool AlUAbsMedia::_onRemoveTrack(AlMessage *msg) {
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        auto it = map.find((*itr)->id());
        if (map.end() != it) {
            map.erase(it);
        }
    }
    return true;
}

bool AlUAbsMedia::_onEnd(AlMessage *msg) {
    mCurTimeInUS = 0;
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        auto decoder = findDecoderByClip(itr->get());
        decoder->seek(0, AbsDecoder::kSeekMode::EXACT);
        decoder->start();
    }
    return true;
}

bool AlUAbsMedia::_onBeat(AlMessage *msg) {
    if (!shouldDecodeFrame()) {
        return true;
    }
    mCurTimeInUS = msg->arg2;
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    if (msg->arg1) {
        onActionSeek(clips, mCurTimeInUS);
        return true;
    }
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        auto *clip = itr->get();
        if (onInterruptClip(clip)) {
            continue;
        }
        auto decoder = findDecoderByClip(clip);
        if (nullptr == decoder) {
            continue;
        }
        while (decoder) {
            HwAbsMediaFrame *frame = nullptr;
            HwResult ret = _grab(clip, decoder, &frame, mCurTimeInUS);
            if (Hw::MEDIA_EOF == ret) {
                onClipEOF(clip);
                AlLogI(TAG, "EOF");
                break;
            }
            if (Hw::MEDIA_WAIT == ret) {
                AlLogW(TAG, "Grab retry. cur(%lld)", mCurTimeInUS);
                continue;
            }
            if (Hw::OK != ret) {
//                AlLogW(TAG, "Grab failed.");
                break;
            }
            if (nullptr == frame && Hw::MEDIA_EOF != ret) {
                continue;
            }
            if (onDispatchFrame(clip, frame, mCurTimeInUS)) {
                continue;
            }
            break;
        }
    }
    onBeatFinish(clips);
    return true;
}

void AlUAbsMedia::_create(AlMediaClip *clip, int64_t &duration, int64_t &frameDuration) {
    if (nullptr == clip || AlIdentityCreator::NONE_ID == clip->id()) {
        AlLogE(TAG, "failed. Invalid clip.");
        return;
    }
    if (AlAbsInputDescriptor::kType::FILE != clip->getInputDescriptor()->type()) {
        AlLogE(TAG, "failed. Not support input type.");
        return;
    }
    auto decoder = createDecoder(clip, frameDuration);
    duration = decoder->getDuration();

    decoder->start();
    auto timeInUS = std::min<int64_t>(mCurTimeInUS, duration);
    timeInUS = std::max<int64_t>(0, timeInUS);
    decoder->seek(timeInUS);
    map.insert(make_pair(clip->id(), std::move(decoder)));
}

HwResult AlUAbsMedia::_grab(AlMediaClip *clip, std::shared_ptr<AbsDecoder> decoder,
                            HwAbsMediaFrame **frame, int64_t timeInUS) {
    auto itr = mLastFrameMap.find(clip->id());
    if (mLastFrameMap.end() != itr) {
        if (timeInUS < clip->getSeqIn() + itr->second->getPts()) {
            AlLogD(TAG, "Skip. Want %" PRId64 ", but %" PRId64, timeInUS,
                   clip->getSeqIn() + itr->second->getPts());
            return Hw::FAILED;
        } else {
            *frame = itr->second;
            mLastFrameMap.erase(itr);
            return Hw::OK;
        }
    }
    HwResult ret = decoder->grab(frame);
    while (nullptr != *frame) {
        if ((*frame)->flags() & AlMediaDef::FLAG_EOF) {
            decoder->seek(0, AbsDecoder::kSeekMode::EXACT);
            decoder->start();
            AlLogI(TAG, "FLAG_EOF");
        } else if ((*frame)->flags() & AlMediaDef::FLAG_SEEK_DONE) {
            AlLogI(TAG, "FLAG_SEEK_DONE");
        } else {
            break;
        }
        ret = decoder->grab(frame);
    }
    if (Hw::OK != ret || nullptr == *frame) {
        return ret;
    }
    if ((AlMediaTrack::kType::VIDEO == type && (*frame)->isVideo()) ||
        (AlMediaTrack::kType::AUDIO == type && (*frame)->isAudio())) {
        if (timeInUS < clip->getSeqIn() + (*frame)->getPts()) {
            mLastFrameMap.insert(std::make_pair(clip->id(), *frame));
            return Hw::FAILED;
        }
        return Hw::OK;
    }
    return Hw::FAILED;
}

std::shared_ptr<AbsDecoder> AlUAbsMedia::findDecoderByClip(AlMediaClip *clip) {
    if (nullptr == clip) {
        return nullptr;
    }
    auto itr = map.find(clip->id());
    if (map.end() == itr) {
        return nullptr;
    }
    return itr->second;
}