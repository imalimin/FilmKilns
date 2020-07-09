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
        auto msg1 = AlMessage::obtain(MSG_AUDIOS_TRACK_ADD_DONE);
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
    auto curTimeMap = mCurTimeMap;
    mCurTimeMap.clear();
    mixer->clearSelect();
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
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
        _correct(clip, decoder, curTimeMap);

        bool offsetDone = false;
        while (decoder) {
            HwAbsMediaFrame *frame = nullptr;
            HwResult ret = decoder->grab(&frame);
            if (Hw::MEDIA_EOF == ret) {
                _putSilence(clip, FRAME_SIZE - count);
                AlLogI(TAG, "EOF");
                break;
            }
            if (nullptr == frame && Hw::MEDIA_EOF != ret) {
                continue;
            }
            if (frame->isAudio()) {
//                if (!offsetDone) {
//                    if (Hw::OK == _offsetDynamic(clip, decoder, frame->getPts())) {
//                        offsetDone = true;
//                    }
//                }

                mixer->put(clip->id(), dynamic_cast<HwAudioFrame *>(frame));
                mixer->select(clip->id());
                if (mixer->samplesOfTrack(itr->get()->id()) < FRAME_SIZE) {
                    continue;
                }
                mCurTimeMap.insert(std::make_pair(clip->id(), frame->getPts()));
            }
            break;
        }
    }
    HwAbsMediaFrame *frame = nullptr;
    if (Hw::OK == mixer->pop(FRAME_SIZE, &frame)) {
        AlMessage *msg0 = AlMessage::obtain(EVENT_SPEAKER_FEED);
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
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        _seek(_findDecoder(itr->get()), 0);
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

void AlUAudios::_seek(AbsAudioDecoder *decoder, int64_t timeInUS) {
    if (decoder) {
        AlLogI(TAG, "seek to %" PRId64, timeInUS);
        decoder->seek(timeInUS);
        decoder->start();
    }
}

HwResult AlUAudios::_correct(AlMediaClip *clip, AbsAudioDecoder *decoder,
                             std::map<AlID, int64_t> &map) {
    int64_t curTime = 0;
    auto itr = map.find(clip->id());
    if (map.end() != itr) {
        curTime = std::min(itr->second, decoder->getDuration());
    }
    int64_t delta = curTime + clip->getSeqIn() - mCurTimeInUS;
    float scale = std::abs(delta) * decoder->getSampleHz() /
                  (decoder->getSamplesPerBuffer() * 1e6f);
    if (scale >= 3) {
        AlLogW(TAG, "Seek clip(%d) from(%" PRId64 ") to(%" PRId64 ")",
               clip->id(), curTime, mCurTimeInUS - clip->getSeqIn());
        _seek(decoder, mCurTimeInUS - clip->getSeqIn());
    }
    return Hw::OK;
}

HwResult AlUAudios::_offsetDynamic(AlMediaClip *clip, AbsAudioDecoder *decoder,
                                   int64_t curFramePts) {
    int64_t delta = curFramePts + clip->getSeqIn() - mCurTimeInUS;
    float scale = std::abs(delta) * decoder->getSampleHz() /
                  (decoder->getSamplesPerBuffer() * 1e6f);
    if (scale < 2 && 0 != delta) {
        if (delta > 0) {
            int32_t nb = std::abs(delta) * format.getSampleRate() / 1e6;
            nb = std::min(nb, FRAME_SIZE);
            int64_t len = nb * format.getChannels()
                          * HwAbsMediaFrame::getBytesPerSample(format.getFormat());
            auto *data = new uint8_t[len];

            memset(data, 0, len);
            mixer->put(clip->id(), format, data, nb);
            AlLogW(TAG, "Offset clip(%d) -%d", clip->id(), nb);
            delete[] data;
        } else if (delta < 0) {
            int32_t nb = std::abs(delta) * format.getSampleRate() / 1e6;
            AlLogW(TAG, "Offset clip(%d) +%d", clip->id(), nb);
        }
        return Hw::OK;
    }
    return Hw::FAILED;
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
