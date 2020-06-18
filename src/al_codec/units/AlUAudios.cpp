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

#define TAG "AlUAudios"

AlUAudios::AlUAudios(const std::string alias) : Unit(alias) {
    al_reg_msg(MSG_AUDIOS_ADD, AlUAudios::_onAddTrack);
}

AlUAudios::~AlUAudios() {

}

bool AlUAudios::onCreate(AlMessage *msg) {
    return true;
}

bool AlUAudios::onDestroy(AlMessage *msg) {
    auto itr = map.begin();
    while (map.end() != itr) {
        itr->second->stop();
        ++itr;
    }
    map.clear();
    return true;
}

bool AlUAudios::_onAddTrack(AlMessage *msg) {
    AlLogI(TAG, "");
    auto *clip = dynamic_cast<AlMediaClip *>(msg->sp.get());
    auto duration = _create(clip);
    if (duration > 0) {
        auto msg1 = AlMessage::obtain(MSG_AUDIOS_TRACK_ADD_NOTIFY);
        msg1->arg1 = clip->id();
        msg1->arg2 = duration;
        postMessage(msg1);
    }
    return true;
}

int64_t AlUAudios::_create(AlMediaClip *clip) {
    if (nullptr == clip || AlIdentityCreator::NONE_ID == clip->id()) {
        AlLogE(TAG, "failed. Invalid clip.");
        return 0;
    }
    if (AlAbsInputDescriptor::kType::FILE != clip->getInputDescriptor()->type()) {
        AlLogE(TAG, "failed. Not support input type.");
        return 0;
    }
    std::string path = clip->getInputDescriptor()->path();
    if (StringUtils::isEmpty(&path)) {
        AlLogE(TAG, "failed. Invalid path(%s).", path.c_str());
        return 0;
    }
    std::unique_ptr<AsynAudioDecoder> decoder = std::make_unique<AsynAudioDecoder>();
    if (!decoder->prepare(path)) {
        AlLogE(TAG, "failed. Decoder prepare failed.");
        return 0;
    }
    auto duration = decoder->getDuration();
    decoder->start();
    AlLogI(TAG, "%" PRId64 ", %d, %d, %d, %s",
           decoder->getDuration(),
           decoder->getChannels(),
           decoder->getSampleHz(),
           decoder->getSampleFormat(), path.c_str());
    map.insert(make_pair(clip->id(), std::move(decoder)));
    return duration;
}
