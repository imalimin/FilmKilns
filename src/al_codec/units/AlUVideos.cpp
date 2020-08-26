/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlUVideos.h"
#include "StringUtils.h"
#include "AsynVideoDecoder.h"
#include "AlSize.h"
#include "AlBuffer.h"

#define TAG "AlUVideos"

AlUVideos::AlUVideos(const std::string alias)
        : AlUAbsMedia(alias, AlMediaTrack::kType::VIDEO) {
    al_reg_msg(MSG_VIDEOS_TRACK_ADD, AlUVideos::_onAddTrack);
    al_reg_msg(MSG_VIDEOS_TRACK_REMOVE, AlUVideos::_onRemoveTrack);
    al_reg_msg(MSG_SEQUENCE_BEAT_VIDEO, AlUVideos::_onBeat);
    al_reg_msg(MSG_VIDEOS_END, AlUVideos::_onEnd);
    al_reg_msg(EVENT_LAYER_QUERY_ID_NOTIFY, AlUVideos::_onLayerDone);
}

AlUVideos::~AlUVideos() {
}

bool AlUVideos::onCreate(AlMessage *msg) {
    return AlUAbsMedia::onCreate(msg);
}

bool AlUVideos::onDestroy(AlMessage *msg) {
    bool ret = AlUAbsMedia::onDestroy(msg);
    mLayerMap.clear();
    return ret;
}

bool AlUVideos::shouldDecodeFrame() {
    ignoreClearClips.clear();
    return true;
}

bool AlUVideos::onInterruptClip(AlMediaClip *clip) {
    ignoreClearClips.emplace_back(clip->id());
    return false;
}

bool AlUVideos::onClipEOF(AlMediaClip *clip) {
    return false;
}

bool AlUVideos::onDispatchFrame(AlMediaClip *clip, HwAbsMediaFrame *frame, int64_t timeInUS) {
    if (frame->isVideo()) {
        int32_t layer = _findLayer(clip);
        if (AlIdentityCreator::NONE_ID != layer) {
            _updateLayer(clip, dynamic_cast<HwVideoFrame *>(frame));
        }
    }
    return false;
}

bool AlUVideos::onBeatFinish(std::shared_ptr<AlVector<std::shared_ptr<AlMediaClip>>> clips) {
    if (!mLayerMap.empty()) {
        _clearLayers(ignoreClearClips);
        postEvent(AlMessage::obtain(EVENT_COMMON_INVALIDATE, AlMessage::QUEUE_MODE_UNIQUE));
    }
    return false;
}

bool AlUVideos::_onLayerDone(AlMessage *msg) {
    mLayerMap[msg->action] = msg->arg1;
    return true;
}

std::shared_ptr<AbsDecoder> AlUVideos::createDecoder(AlMediaClip *clip,
                                                     int64_t &frameDuration) {
    std::string path = clip->getInputDescriptor()->path();
    if (StringUtils::isEmpty(&path)) {
        AlLogE(TAG, "failed. Invalid path(%s).", path.c_str());
        return nullptr;
    }
    auto decoder = std::make_shared<AsynVideoDecoder>(true);
    if (!decoder->prepare(path)) {
        AlLogE(TAG, "failed. Decoder prepare failed.");
        return nullptr;
    }
    _addLayer(clip, decoder->width(), decoder->height());
    auto frameSize = decoder->getSamplesPerBuffer();
    frameDuration = 1e6 * frameSize / decoder->getSampleHz();
    AlLogI(TAG, "%" PRId64 ", %d, %d, %d, %s",
           decoder->getDuration(),
           decoder->getChannels(),
           decoder->getSampleHz(),
           decoder->getSampleFormat(), path.c_str());
    return decoder;
}

void AlUVideos::onActionSeek(std::shared_ptr<AlVector<std::shared_ptr<AlMediaClip>>> clips, int64_t timeInUS) {
    std::vector<AlID> ignoreClearClips;
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        auto *clip = itr->get();
        ignoreClearClips.emplace_back(clip->id());
        auto decoder = findDecoderByClip(clip);
        _seek(decoder, timeInUS);
        if (nullptr == decoder) {
            continue;
        }
        while (decoder) {
            HwAbsMediaFrame *frame = nullptr;
            HwResult ret = decoder->grab(&frame);
            while (Hw::MEDIA_WAIT == ret || nullptr == frame) {
                ret = decoder->grab(&frame);
                if (Hw::MEDIA_EOF == ret ||
                    (nullptr != frame && frame->flags() & AlMediaDef::FLAG_EOF)) {
                    AlLogI(TAG, "FLAG_EOF");
                    break;
                }
            }
            while (nullptr != frame) {
                if (frame->flags() & AlMediaDef::FLAG_SEEK_DONE) {
                    AlLogI(TAG, "FLAG_SEEK_DONE");
                } else {
                    break;
                }
                while (Hw::MEDIA_WAIT == ret) {
                    ret = decoder->grab(&frame);
                    if (Hw::MEDIA_EOF == ret ||
                        (nullptr != frame && frame->flags() & AlMediaDef::FLAG_EOF)) {
                        AlLogI(TAG, "FLAG_EOF");
                        break;
                    }
                }
            }
            if (Hw::OK != ret || nullptr == frame) {
                if (Hw::MEDIA_WAIT == ret) {
                    AlLogI(TAG, "MEDIA_WAIT");
                } else {
                    AlLogI(TAG, "frame is null");
                }
                break;
            }
            if (frame->isVideo()) {
                int32_t layer = _findLayer(clip);
                if (AlIdentityCreator::NONE_ID != layer) {
                    AlLogI(TAG, "seek render %" PRId64, frame->getPts());
                    _updateLayer(clip, dynamic_cast<HwVideoFrame *>(frame));
                }
            }
            break;
        }
    }
    if (!mLayerMap.empty()) {
        _clearLayers(ignoreClearClips);
        postEvent(AlMessage::obtain(EVENT_COMMON_INVALIDATE, AlMessage::QUEUE_MODE_UNIQUE));
    }
}

void AlUVideos::_seek(std::shared_ptr<AbsDecoder> decoder, int64_t timeInUS) {
    if (decoder) {
        AlLogI(TAG, "seek to %" PRId64, timeInUS);
        decoder->seek(timeInUS, AbsDecoder::kSeekMode::EXACT);
        decoder->start();
    }
}

int32_t AlUVideos::_findLayer(AlMediaClip *clip) {
    if (nullptr == clip) {
        return AlIdentityCreator::NONE_ID;
    }
    auto itr = mLayerMap.find(clip->id());
    if (mLayerMap.end() == itr) {
        return AlIdentityCreator::NONE_ID;
    }
    return itr->second;
}

void AlUVideos::_addLayer(AlMediaClip *clip, int32_t width, int32_t height) {
    auto *msg = AlMessage::obtain(MSG_LAYER_ADD_EMPTY);
    msg->action = clip->id();
    msg->obj = new AlSize(width, height);
    postMessage(msg);
}

void AlUVideos::_updateLayer(AlMediaClip *clip, HwVideoFrame *frame) {
    if (nullptr == clip || nullptr == frame) {
        AlLogE(TAG, "failed.");
        return;
    }
    auto itr = mLayerMap.find(clip->id());
    if (mLayerMap.end() == itr) {
        AlLogE(TAG, "failed.");
        return;
    }
    auto *msg = AlMessage::obtain(MSG_LAYER_UPDATE_WITH_BUF);
    if (HwFrameFormat::HW_IMAGE_RGBA == frame->getFormat()) {
        msg->arg2 = static_cast<int64_t>(AlColorFormat::RGBA);
    } else if (HwFrameFormat::HW_IMAGE_NV12 == frame->getFormat()) {
        msg->arg2 = static_cast<int64_t>(AlColorFormat::NV12);
    } else if (HwFrameFormat::HW_IMAGE_YV12 == frame->getFormat()) {
        msg->arg2 = static_cast<int64_t>(AlColorFormat::YV12);
    } else {
        msg->arg2 = static_cast<int64_t>(AlColorFormat::NONE);
    }
    msg->arg1 = itr->second;
    msg->obj = AlBuffer::wrap(frame->data(), frame->size());
    msg->sp = std::make_shared<AlSize>(frame->getWidth(), frame->getHeight());
    postMessage(msg);
}

void AlUVideos::_clearLayers(std::vector<AlID> &ignoreClips) {
    std::for_each(mLayerMap.begin(), mLayerMap.end(),
                  [this, ignoreClips](std::map<AlID, int32_t>::reference &it) {
                      if (ignoreClips.end() ==
                          std::find(ignoreClips.begin(), ignoreClips.end(), it.first)) {
                          this->_clearLayer(it.second);
                      }
                  });
}

void AlUVideos::_clearLayer(int32_t layerID) {
    auto *msg = AlMessage::obtain(MSG_LAYER_UPDATE_CLEAR);
    msg->arg1 = layerID;
    postMessage(msg);
}
