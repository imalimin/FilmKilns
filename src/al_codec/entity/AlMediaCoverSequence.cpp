/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlMediaCoverSequence.h"

AlMediaCoverSequence::AlMediaCoverSequence() : AlParcelable() {

}

AlMediaCoverSequence::~AlMediaCoverSequence() {
    covers.clear();
}

void AlMediaCoverSequence::writeToParcel(std::shared_ptr<AlParcel> parcel) {
    parcel->writeInt(covers.size());
    for (auto &it:covers) {
        it->writeToParcel(parcel);
    }
}