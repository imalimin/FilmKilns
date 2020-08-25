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
        : AlUAbsMedia(alias, AlMediaTrack::kType::AUDIO),
          format(HwFrameFormat::HW_SAMPLE_S16, 2, 44100) {
    al_reg_msg(MSG_AUDIOS_TRACK_ADD, AlUAudios::_onAddTrack);
    al_reg_msg(MSG_AUDIOS_TRACK_REMOVE, AlUAudios::_onRemoveTrack);
    al_reg_msg(MSG_SEQUENCE_BEAT_AUDIO, AlUAudios::_onBeat);
    al_reg_msg(MSG_AUDIOS_END, AlUAudios::_onEnd);
}

AlUAudios::~AlUAudios() {
}

bool AlUAudios::onCreate(AlMessage *msg) {
    bool ret = AlUAbsMedia::onCreate(msg);
    mixer = new AlAudioPoolMixer(format);
    return ret;
}

bool AlUAudios::onDestroy(AlMessage *msg) {
    bool ret = AlUAbsMedia::onDestroy(msg);
    delete pSilenceBuf;
    pSilenceBuf = nullptr;
    delete mixer;
    mixer = nullptr;
    return ret;
}

bool AlUAudios::shouldDecodeFrame() {
    if (nullptr == mixer) {
        return false;
    }
    mixer->clearSelect();
    return true;
}

bool AlUAudios::onInterruptClip(AlMediaClip *clip) {
    auto count = mixer->samplesOfTrack(clip->id());
    if (count >= FRAME_SIZE) {
        mixer->select(clip->id());
        return true;
    }
    if (clip->getInputDescriptor()->equals(AlFileDescriptor::EMPTY)) {
        _putSilence(clip, FRAME_SIZE - count);
        return true;
    }
    return false;
}

bool AlUAudios::onClipEOF(AlMediaClip *clip) {
    auto count = mixer->samplesOfTrack(clip->id());
    _putSilence(clip, FRAME_SIZE - count);
    return false;
}

bool AlUAudios::onDispatchFrame(AlMediaClip *clip, HwAbsMediaFrame *frame, int64_t timeInUS) {
    if (frame->isAudio()) {
        mixer->put(clip->id(), dynamic_cast<HwAudioFrame *>(frame));
        mixer->select(clip->id());
        if (mixer->samplesOfTrack(clip->id()) < FRAME_SIZE) {
            return true;
        }
//        _setCurTimestamp(clip, frame->getPts());
    }
    return false;
}

bool AlUAudios::onBeatFinish(std::shared_ptr<AlVector<std::shared_ptr<AlMediaClip>>> clips) {
    HwAbsMediaFrame *frame = nullptr;
    if (Hw::OK == mixer->pop(FRAME_SIZE, &frame)) {
        AlMessage *msg0 = AlMessage::obtain(EVENT_SPEAKER_FEED, AlMessage::QUEUE_MODE_UNIQUE);
        msg0->obj = frame->clone();
        postMessage(msg0);

        AlMessage *msg1 = AlMessage::obtain(MSG_TIMELINE_ADD);
        msg1->arg1 = FRAME_SIZE;
        postMessage(msg1);
    } else {
        AlLogI(TAG, "Leak samples! %d clips", clips->size());
    }
    return false;
}
void AlUAudios::onActionSeek(std::shared_ptr<AlVector<std::shared_ptr<AlMediaClip>>> clips, int64_t timeInUS) {
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        _seek(findDecoderByClip(itr->get()), timeInUS);
    }
}

void AlUAudios::_seek(std::shared_ptr<AbsDecoder> decoder, int64_t timeInUS) {
    if (decoder) {
        AlLogI(TAG, "seek to %" PRId64, timeInUS);
        decoder->seek(timeInUS, AbsDecoder::kSeekMode::EXACT);
        decoder->start();
    }
}

std::shared_ptr<AbsDecoder> AlUAudios::createDecoder(AlMediaClip *clip,
                                                     int64_t &frameDuration) {
    std::string path = clip->getInputDescriptor()->path();
    if (StringUtils::isEmpty(&path)) {
        AlLogE(TAG, "failed. Invalid path(%s).", path.c_str());
        return nullptr;
    }
    auto decoder = std::make_shared<AsynAudioDecoder>();
    decoder->setOutSampleFormat(format);
    if (!decoder->prepare(path)) {
        AlLogE(TAG, "failed. Decoder prepare failed.");
        return nullptr;
    }
    auto frameSize = decoder->getSamplesPerBuffer();
    frameDuration = 1e6 * frameSize / decoder->getSampleHz();
    AlLogI(TAG, "%" PRId64 ", %d, %d, %d, %s",
           decoder->getDuration(),
           decoder->getChannels(),
           decoder->getSampleHz(),
           decoder->getSampleFormat(), path.c_str());
    return decoder;
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
