/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUVideos.h"
#include "StringUtils.h"
#include "AsynVideoDecoder.h"
#include "AlVector.h"

#define TAG "AlUVideos"

AlUVideos::AlUVideos(const std::string alias)
        : Unit(alias) {
    al_reg_msg(MSG_VIDEOS_TRACK_ADD, AlUVideos::_onAddTrack);
    al_reg_msg(MSG_VIDEOS_TRACK_REMOVE, AlUVideos::_onRemoveTrack);
    al_reg_msg(MSG_SEQUENCE_BEAT_VIDEO, AlUVideos::_onBeat);
    al_reg_msg(MSG_VIDEOS_END, AlUVideos::_onEnd);
}

AlUVideos::~AlUVideos() {
}

bool AlUVideos::onCreate(AlMessage *msg) {
    return true;
}

bool AlUVideos::onDestroy(AlMessage *msg) {
    for_each(map.begin(), map.end(),
             [](std::map<AlID, std::unique_ptr<AbsVideoDecoder>>::reference &it) {
                 it.second->stop();
             });
    map.clear();
    return true;
}

bool AlUVideos::_onAddTrack(AlMessage *msg) {
    AlLogI(TAG, "");
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

bool AlUVideos::_onRemoveTrack(AlMessage *msg) {
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        auto it = map.find((*itr)->id());
        if (map.end() != it) {
            map.erase(it);
        }
    }
    return true;
}

bool AlUVideos::_onBeat(AlMessage *msg) {
    mCurTimeInUS = msg->arg2;
    auto curTimeMap = mCurTimeMap;
    mCurTimeMap.clear();
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        auto *clip = itr->get();
        auto decoder = _findDecoder(clip);
        if (nullptr == decoder) {
            continue;
        }
        while (decoder) {
            HwAbsMediaFrame *frame = nullptr;
            HwResult ret = decoder->grab(&frame);
            if (Hw::MEDIA_EOF == ret) {
                AlLogI(TAG, "EOF");
                break;
            }
            if (nullptr == frame && Hw::MEDIA_EOF != ret) {
                continue;
            }
            if (frame->isVideo()) {
                mCurTimeMap.insert(std::make_pair(clip->id(), frame->getPts()));
            }
            break;
        }
    }
    return true;
}

bool AlUVideos::_onEnd(AlMessage *msg) {
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        _seek(_findDecoder(itr->get()), 0);
    }
    return false;
}

void AlUVideos::_create(AlMediaClip *clip, int64_t &duration, int64_t &frameDuration) {
    if (nullptr == clip || AlIdentityCreator::NONE_ID == clip->id()) {
        AlLogE(TAG, "failed. Invalid clip.");
        return;
    }
    if (AlAbsInputDescriptor::kType::FILE != clip->getInputDescriptor()->type()) {
        AlLogE(TAG, "failed. Not support input type.");
        return;
    }
    std::string path = clip->getInputDescriptor()->path();
    if (StringUtils::isEmpty(&path)) {
        AlLogE(TAG, "failed. Invalid path(%s).", path.c_str());
        return;
    }
    std::unique_ptr<AsynVideoDecoder> decoder = std::make_unique<AsynVideoDecoder>();
    if (!decoder->prepare(path)) {
        AlLogE(TAG, "failed. Decoder prepare failed.");
        return;
    }

    duration = decoder->getDuration();
    auto frameSize = decoder->getSamplesPerBuffer();
    frameDuration = 1e6 * frameSize / decoder->getSampleHz();

    decoder->start();
    auto timeInUS = std::min<int64_t>(mCurTimeInUS, duration);
    timeInUS = std::max<int64_t>(0, timeInUS);
    decoder->seek(timeInUS);
    AlLogI(TAG, "%" PRId64 ", %d, %d, %d, %s",
           decoder->getDuration(),
           decoder->getChannels(),
           decoder->getSampleHz(),
           decoder->getSampleFormat(), path.c_str());
    map.insert(make_pair(clip->id(), std::move(decoder)));
}

void AlUVideos::_seek(AbsAudioDecoder *decoder, int64_t timeInUS) {
    if (decoder) {
        AlLogI(TAG, "seek to %" PRId64, timeInUS);
        decoder->seek(timeInUS);
        decoder->start();
    }
}

AbsAudioDecoder *AlUVideos::_findDecoder(AlMediaClip *clip) {
    if (nullptr == clip) {
        return nullptr;
    }
    auto itr = map.find(clip->id());
    if (map.end() == itr) {
        return nullptr;
    }
    return itr->second.get();
}
