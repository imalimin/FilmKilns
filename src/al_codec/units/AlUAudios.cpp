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
#include "AlBuffer.h"

#define TAG "AlUAudios"

AlUAudios::AlUAudios(const std::string alias)
        : Unit(alias),
          format(HwFrameFormat::HW_SAMPLE_S16, 2, 44100) {
    al_reg_msg(MSG_AUDIOS_TRACK_ADD, AlUAudios::_onAddTrack);
    al_reg_msg(MSG_AUDIOS_TRACK_REMOVE, AlUAudios::_onRemoveTrack);
    al_reg_msg(MSG_SEQUENCE_BEAT_AUDIO, AlUAudios::_onBeat);
    al_reg_msg(MSG_AUDIOS_END, AlUAudios::_onEnd);
}

AlUAudios::~AlUAudios() {
}

bool AlUAudios::onCreate(AlMessage *msg) {
    mixer = new AlAudioPoolMixer(format);
    return true;
}

bool AlUAudios::onDestroy(AlMessage *msg) {
    delete pSilenceBuf;
    pSilenceBuf = nullptr;
    delete mixer;
    mixer = nullptr;
    for (auto &it : map) {
        it.second->stop();
    }
    map.clear();
    mCurTimeMap.clear();
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
        auto msg1 = AlMessage::obtain(MSG_SEQUENCE_TRACK_SET_DURATION);
        msg1->sp = clip;
        postMessage(msg1);
    }
    return true;
}

bool AlUAudios::_onRemoveTrack(AlMessage *msg) {
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        auto it = map.find((*itr)->id());
        if (map.end() != it) {
            map.erase(it);
        }
    }
    return true;
}

bool AlUAudios::_onBeat(AlMessage *msg) {
    if (nullptr == mixer) {
        return true;
    }
    mCurTimeInUS = msg->arg2;
    mixer->clearSelect();
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    if (msg->arg1) {
        _seek(clips, mCurTimeInUS);
        return true;
    }
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        auto *clip = itr->get();
        auto count = mixer->samplesOfTrack(clip->id());
        if (count >= FRAME_SIZE) {
            mixer->select(clip->id());
            continue;
        }
        if (clip->getInputDescriptor()->equals(AlFileDescriptor::EMPTY)) {
            _putSilence(clip, FRAME_SIZE - count);
            continue;
        }
        auto decoder = _findDecoder(clip);
        if (nullptr == decoder) {
            continue;
        }

        while (decoder) {
            HwAbsMediaFrame *frame = nullptr;
            HwResult ret = _grab(clip, decoder, &frame, mCurTimeInUS);
            if (Hw::MEDIA_EOF == ret) {
                _putSilence(clip, FRAME_SIZE - count);
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
            if (frame->isAudio()) {
                mixer->put(clip->id(), dynamic_cast<HwAudioFrame *>(frame));
                mixer->select(clip->id());
                if (mixer->samplesOfTrack(itr->get()->id()) < FRAME_SIZE) {
                    continue;
                }
                _setCurTimestamp(clip, frame->getPts());
            }
            break;
        }
    }
    HwAbsMediaFrame *frame = nullptr;
    if (Hw::OK == mixer->pop(FRAME_SIZE, &frame)) {
        AlMessage *msg0 = AlMessage::obtain(EVENT_SPEAKER_FEED, AlMessage::QUEUE_MODE_UNIQUE);
        msg0->obj = frame->clone();
        postEvent(msg0);

        AlMessage *msg1 = AlMessage::obtain(MSG_TIMELINE_ADD);
        msg1->arg1 = FRAME_SIZE;
        postEvent(msg1);
    } else {
        AlLogI(TAG, "Leak samples! %d clips", clips->size());
    }
    return true;
}

bool AlUAudios::_onEnd(AlMessage *msg) {
    mCurTimeInUS = 0;
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        _seek(_findDecoder(itr->get()), 0);
    }
    return true;
}

void AlUAudios::_seek(std::shared_ptr<AlVector<std::shared_ptr<AlMediaClip>>> clips,
                      int64_t timeInUS) {
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        _seek(_findDecoder(itr->get()), timeInUS);
    }
}

void AlUAudios::_seek(AbsAudioDecoder *decoder, int64_t timeInUS) {
    if (decoder) {
        AlLogI(TAG, "seek to %" PRId64, timeInUS);
        decoder->seek(timeInUS, AbsDecoder::kSeekMode::EXACT);
        decoder->start();
    }
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
    decoder->setOutSampleFormat(format);
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

HwResult AlUAudios::_grab(AlMediaClip *clip, AbsAudioDecoder *decoder,
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
            _seek(decoder, 0);
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
    if ((*frame)->isAudio()) {
        if (timeInUS < clip->getSeqIn() + (*frame)->getPts()) {
            mLastFrameMap.insert(std::make_pair(clip->id(), *frame));
            return Hw::FAILED;
        }
        return Hw::OK;
    }
    return Hw::FAILED;

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

HwResult AlUAudios::_putSilence(AlMediaClip *clip, int nbSamples) {
    if (nullptr == clip || nbSamples <= 0) {
        return Hw::FAILED;
    }
    if (nullptr == pSilenceBuf) {
        int64_t len = FRAME_SIZE * format.getChannels()
                      * HwAbsMediaFrame::getBytesPerSample(format.getFormat());
        pSilenceBuf = AlBuffer::alloc(len);
        memset(pSilenceBuf->data(), 0, pSilenceBuf->size());
    }
    mixer->put(clip->id(), format, pSilenceBuf->data(), std::min(nbSamples, FRAME_SIZE));
    mixer->select(clip->id());
    return Hw::SUCCESS;
}

void AlUAudios::_setCurTimestamp(AlMediaClip *clip, int64_t timeInUS) {
    auto itr = mCurTimeMap.find(clip->id());
    if (mCurTimeMap.end() != itr) {
        mCurTimeMap.erase(itr);
    }
    if (timeInUS != INT64_MIN) {
        mCurTimeMap.insert(std::make_pair(clip->id(), timeInUS));
    }
}

int64_t AlUAudios::_getCurTimestamp(AlMediaClip *clip) {
    auto itr = mCurTimeMap.find(clip->id());
    if (mCurTimeMap.end() != itr) {
        return itr->second;
    }
    return INT64_MIN;
}
