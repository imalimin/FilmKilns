/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkMsgState.h"

FkMsgState::FkMsgState() {

}

FkMsgState::~FkMsgState() {

}

void FkMsgState::writeToParcel(std::shared_ptr<AlParcel> parcel) {
    parcel->writeLong(what);
    parcel->writeLong(postTimeInUS);
    parcel->writeLong(handleTimeInUS);
    parcel->writeLong(handleDoneInUS);
    parcel->writeLong(costTimeInUS);
}

void FkMsgState::copyTo(std::shared_ptr<FkMsgState> o) {
    o->postTimeInUS = postTimeInUS;
    o->handleTimeInUS = handleTimeInUS;
    o->handleDoneInUS = handleDoneInUS;
    o->costTimeInUS = costTimeInUS;
}
