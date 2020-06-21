/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUAudios.h"
#include "AlLogcat.h"
#include "AsynAudioDecoder.h"
#include "StringUtils.h"
#include "AlVector.h"

#define TAG "AlUAudios"

AlUAudios::AlUAudios(const std::string alias) : Unit(alias) {
    al_reg_msg(MSG_AUDIOS_ADD, AlUAudios::_onAddTrack);
    al_reg_msg(MSG_SEQUENCE_BEAT_AUDIO, AlUAudios::_onBeat);
}

AlUAudios::~AlUAudios() {

}

bool AlUAudios::onCreate(AlMessage *msg) {
    return true;
}

bool AlUAudios::onDestroy(AlMessage *msg) {
    for (auto &it : map) {
        it.second->stop();
    }
    map.clear();
    return true;
}

bool AlUAudios::_onAddTrack(AlMessage *msg) {
    AlLogI(TAG, "");
    auto clip = std::static_pointer_cast<AlMediaClip>(msg->sp);
    int64_t duration = 0;
    int64_t frameDuration = 0;
    _create(clip.get(), duration, frameDuration);
    if (duration > 0) {
        clip->setDuration(duration);
        clip->setFrameDuration(frameDuration);
        auto msg1 = AlMessage::obtain(MSG_AUDIOS_TRACK_ADD_NOTIFY);
        msg1->sp = clip;
        postMessage(msg1);
    }
    return true;
}

bool AlUAudios::_onBeat(AlMessage *msg) {
    auto timeInUS = msg->arg2;
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    HwAbsMediaFrame *frame = nullptr;
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        auto decoder = _findDecoder(itr->get());
        if (decoder && 0 == timeInUS) {
            AlLogI(TAG, "seek 0.");
            decoder->seek(timeInUS);
            decoder->start();
        }
        while (decoder) {
            HwResult ret = decoder->grab(&frame);
            if (Hw::MEDIA_EOF == ret) {
                AlLogI(TAG, "EOF");
                break;
            }
            if (nullptr == frame && Hw::MEDIA_EOF != ret) {
                continue;
            }
            if (frame->isAudio()) {
                AlMessage *msg0 = AlMessage::obtain(EVENT_SPEAKER_FEED);
                msg0->obj = frame->clone();
                postEvent(msg0);
            }
            break;
        }
    }
    return true;
}

void AlUAudios::_create(AlMediaClip *clip, int64_t &duration, int64_t &frameDuration) {
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
    std::unique_ptr<AsynAudioDecoder> decoder = std::make_unique<AsynAudioDecoder>();
    if (!decoder->prepare(path)) {
        AlLogE(TAG, "failed. Decoder prepare failed.");
        return;
    }

    duration = decoder->getDuration();
    auto frameSize = decoder->getSamplesPerBuffer();
    if (frameSize <= 0) {
        frameSize = 1024;
    }
    frameDuration = 1e6 * frameSize / decoder->getSampleHz();

    decoder->start();
    AlLogI(TAG, "%" PRId64 ", %d, %d, %d, %s",
           decoder->getDuration(),
           decoder->getChannels(),
           decoder->getSampleHz(),
           decoder->getSampleFormat(), path.c_str());
    map.insert(make_pair(clip->id(), std::move(decoder)));
}

AbsAudioDecoder *AlUAudios::_findDecoder(AlMediaClip *clip) {
    if (nullptr == clip) {
        return nullptr;
    }
    auto itr = map.find(clip->id());
    if (map.end() == itr) {
        return nullptr;
    }
    return itr->second.get();
}
