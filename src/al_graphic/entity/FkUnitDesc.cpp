/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkUnitDesc.h"

FkUnitDesc::FkUnitDesc() : AlParcelable() {

}

FkUnitDesc::~FkUnitDesc() {

}

void FkUnitDesc::writeToParcel(std::shared_ptr<AlParcel> parcel) {
    parcel->writeString(name);
    parcel->writeLongArray(msgVec);
}
