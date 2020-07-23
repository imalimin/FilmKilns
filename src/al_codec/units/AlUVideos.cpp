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
        : Unit(alias) {
    al_reg_msg(MSG_VIDEOS_TRACK_ADD, AlUVideos::_onAddTrack);
    al_reg_msg(MSG_VIDEOS_TRACK_REMOVE, AlUVideos::_onRemoveTrack);
    al_reg_msg(MSG_SEQUENCE_BEAT_VIDEO, AlUVideos::_onBeat);
    al_reg_msg(MSG_VIDEOS_END, AlUVideos::_onEnd);
    al_reg_msg(EVENT_LAYER_QUERY_ID_NOTIFY, AlUVideos::_onLayerDone);
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
    mLayerMap.clear();
    mLastFrameMap.clear();
    return true;
}

bool AlUVideos::_onAddTrack(AlMessage *msg) {
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
    std::vector<AlID> ignoreClips;
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        auto *clip = itr->get();
        ignoreClips.emplace_back(clip->id());
        auto decoder = _findDecoder(clip);
        if (nullptr == decoder) {
            continue;
        }
        auto seekRet = _correct(clip, decoder);
        while (decoder) {
            HwAbsMediaFrame *frame = nullptr;
            HwResult ret = _grab(clip, decoder, &frame, mCurTimeInUS);
            if (Hw::MEDIA_EOF == ret) {
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
            if (frame->isVideo()) {
                int32_t layer = _findLayer(clip);
                if (AlIdentityCreator::NONE_ID != layer) {
                    _updateLayer(clip, dynamic_cast<HwVideoFrame *>(frame));
                }
                _setCurTimestamp(clip, frame->getPts());
            }
            break;
        }
        if (Hw::SUCCESS == seekRet && mCurTimeMap.end() == mCurTimeMap.find(clip->id())) {
            AlLogW(TAG, "Grab frame failed after seek.");
        }
    }
    if (!mLayerMap.empty()) {
        _clearLayers(ignoreClips);
        postEvent(AlMessage::obtain(EVENT_COMMON_INVALIDATE, AlMessage::QUEUE_MODE_UNIQUE));
    }
    return true;
}

bool AlUVideos::_onEnd(AlMessage *msg) {
    mCurTimeInUS = 0;
    auto clips = std::static_pointer_cast<AlVector<std::shared_ptr<AlMediaClip>>>(msg->sp);
    for (auto itr = clips->begin(); clips->end() != itr; ++itr) {
        _seek(_findDecoder(itr->get()), 0);
    }
    return false;
}

bool AlUVideos::_onLayerDone(AlMessage *msg) {
    mLayerMap[msg->action] = msg->arg1;
    return true;
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
    std::unique_ptr<AsynVideoDecoder> decoder = std::make_unique<AsynVideoDecoder>(true);
    if (!decoder->prepare(path)) {
        AlLogE(TAG, "failed. Decoder prepare failed.");
        return;
    }

    _addLayer(clip, decoder->width(), decoder->height());
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

void AlUVideos::_seek(AbsVideoDecoder *decoder, int64_t timeInUS) {
    if (decoder) {
        AlLogI(TAG, "seek to %" PRId64, timeInUS);
        decoder->seek(timeInUS, AbsDecoder::kSeekMode::EXACT);
        decoder->start();
    }
}

AbsVideoDecoder *AlUVideos::_findDecoder(AlMediaClip *clip) {
    if (nullptr == clip) {
        return nullptr;
    }
    auto itr = map.find(clip->id());
    if (map.end() == itr) {
        return nullptr;
    }
    return itr->second.get();
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
    auto *msg = AlMessage::obtain(MSG_LAYER_UPDATE_YUV);
    if (HwFrameFormat::HW_IMAGE_RGBA == frame->getFormat()) {
        msg->what = MSG_LAYER_UPDATE_RGBA;
    }
    msg->arg1 = itr->second;
    msg->obj = AlBuffer::wrap(frame->data(), frame->size());
    msg->sp = std::make_shared<Size>(frame->getWidth(), frame->getHeight());
    postMessage(msg);
}

HwResult AlUVideos::_grab(AlMediaClip *clip, AbsVideoDecoder *decoder,
                          HwAbsMediaFrame **frame, int64_t timeInUS) {
    auto itr = mLastFrameMap.find(clip->id());
    if (mLastFrameMap.end() != itr) {
        if (timeInUS < clip->getSeqIn() + itr->second->getPts()) {
            /// 解决快退的时候，缓存帧时间戳过大导致等待时间过长的问题
//            if (clip->getSeqIn() + itr->second->getPts() - timeInUS >= 1e6) {
//                _setCurTimestamp(clip, itr->second->getPts());
//                mLastFrameMap.erase(itr);
//            }
//            AlLogW(TAG, "Skip frame(%d), cur(%d), %s, %d",
//                   (int) itr->second->getPts(),
//                   (int) timeInUS,
//                   ((AsynVideoDecoder *) decoder)->dump().c_str(),
//                   mCurTimeMap.end() != mCurTimeMap.find(clip->id()));
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
    if ((*frame)->isVideo()) {
        if (timeInUS < clip->getSeqIn() + (*frame)->getPts()) {
            mLastFrameMap.insert(std::make_pair(clip->id(), *frame));
            return Hw::FAILED;
        }
        return Hw::OK;
    }
    return Hw::FAILED;
}

HwResult AlUVideos::_correct(AlMediaClip *clip, AbsVideoDecoder *decoder) {
    int64_t curTime = _getCurTimestamp(clip);
    if (curTime != INT64_MIN) {
        curTime = curTime < decoder->getDuration() ? curTime : 0;
    } else {
        return Hw::FAILED;
    }
    int64_t delta = curTime + clip->getSeqIn() - mCurTimeInUS;
    float scale = std::abs(delta) / 33333;
    if (scale >= 3) {
        auto cache = mLastFrameMap.find(clip->id());
        if (mLastFrameMap.end() != cache) {
            mLastFrameMap.erase(cache);
        }
        auto timeInUS = mCurTimeInUS - clip->getSeqIn();
        AlLogD(TAG, "Seek clip(%d) scale(%f) from %" PRId64 "US to %" PRId64 "US",
               clip->id(), scale, curTime, timeInUS);
        _seek(decoder, timeInUS);
        _setCurTimestamp(clip, timeInUS);
        return Hw::OK;
    }
    return Hw::FAILED;
}

void AlUVideos::_setCurTimestamp(AlMediaClip *clip, int64_t timeInUS) {
    auto itr = mCurTimeMap.find(clip->id());
    if (mCurTimeMap.end() != itr) {
        mCurTimeMap.erase(itr);
    }
    if (timeInUS != INT64_MIN) {
        mCurTimeMap.insert(std::make_pair(clip->id(), timeInUS));
    }
}

int64_t AlUVideos::_getCurTimestamp(AlMediaClip *clip) {
    auto itr = mCurTimeMap.find(clip->id());
    if (mCurTimeMap.end() != itr) {
        return itr->second;
    }
    return INT64_MIN;
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
