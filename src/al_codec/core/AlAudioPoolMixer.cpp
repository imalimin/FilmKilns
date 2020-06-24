/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAudioPoolMixer.h"

AlAudioPoolMixer::AlAudioPoolMixer()
        : Object() {

}

AlAudioPoolMixer::~AlAudioPoolMixer() {
}


HwResult AlAudioPoolMixer::put(int32_t track, HwAudioFrame *frame) {

    return Hw::OK;
}

HwResult AlAudioPoolMixer::request(size_t nbSamples) {
    return Hw::OK;
}

HwResult AlAudioPoolMixer::pop(HwAbsMediaFrame **frame) {

}