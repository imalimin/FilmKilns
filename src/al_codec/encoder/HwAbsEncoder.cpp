/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAbsEncoder.h"

HwAbsEncoder::Desc::Desc()
        : Object(), vID(AlCodec::kID::NONE), aID(AlCodec::kID::NONE),
          type(HwAbsEncoder::kType::SOFT) {

}

HwAbsEncoder::Desc::Desc(AlCodec::kID id, HwAbsEncoder::kType type)
        : Object(), vID(id), aID(AlCodec::kID::AAC), type(type) {

}

HwAbsEncoder::Desc::Desc(const Desc &o) : Object(), vID(o.vID), aID(o.aID), type(o.type) {

}

HwAbsEncoder::Desc::~Desc() {

}

HwAbsEncoder::HwAbsEncoder(const HwAbsEncoder::Desc &desc) : Object(), desc(desc) {

}

HwAbsEncoder::~HwAbsEncoder() {

}

HwAbsEncoder::Desc HwAbsEncoder::getCodecDesc() {
    return desc;
}
