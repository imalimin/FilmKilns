/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAbsEncoder.h"

AlAbsEncoder::Desc::Desc()
        : Object(), vID(AlCodec::kID::NONE), aID(AlCodec::kID::NONE),
          type(AlAbsEncoder::kType::SOFT) {

}

AlAbsEncoder::Desc::Desc(AlCodec::kID id, AlAbsEncoder::kType type)
        : Object(), vID(id), aID(AlCodec::kID::AAC), type(type) {

}

AlAbsEncoder::Desc::Desc(const Desc &o) : Object(), vID(o.vID), aID(o.aID), type(o.type) {

}

AlAbsEncoder::Desc::~Desc() {

}

AlAbsEncoder::AlAbsEncoder(const AlAbsEncoder::Desc &desc) : Object(), desc(desc) {

}

AlAbsEncoder::~AlAbsEncoder() {

}

AlAbsEncoder::Desc AlAbsEncoder::getCodecDesc() {
    return desc;
}
