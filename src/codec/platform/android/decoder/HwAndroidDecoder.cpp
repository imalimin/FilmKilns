/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "HwAndroidDecoder.h"

HwAndroidDecoder::HwAndroidDecoder() : AbsAudioDecoder(), AbsVideoDecoder() {

}

HwAndroidDecoder::~HwAndroidDecoder() {

}

bool HwAndroidDecoder::prepare(string path) {
    return false;
}

int HwAndroidDecoder::width() {
    return 0;
}

int HwAndroidDecoder::height() {
    return 0;
}

int HwAndroidDecoder::getChannels() {
    return 0;
}

int HwAndroidDecoder::getSampleHz() {
    return 0;
}

int HwAndroidDecoder::getSampleFormat() {
    return 0;
}

int HwAndroidDecoder::getSamplesPerBuffer() {
    return 0;
}

void HwAndroidDecoder::seek(int64_t us) {

}

HwResult HwAndroidDecoder::grab(HwAbsMediaFrame **frame) {
    return HwResult(0);
}

int64_t HwAndroidDecoder::getVideoDuration() {
    return 0;
}

int64_t HwAndroidDecoder::getAudioDuration() {
    return 0;
}

int64_t HwAndroidDecoder::getDuration() {
    return 0;
}

void HwAndroidDecoder::start() {

}

void HwAndroidDecoder::pause() {

}

void HwAndroidDecoder::stop() {

}
