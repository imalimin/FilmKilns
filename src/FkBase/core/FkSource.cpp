/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkSource.h"

FK_IMPL_CLASS_TYPE(FkSource, FkObject)
FK_IMPL_CLASS_TYPE(FkSourceAllocator, FkObject)

FkSource::FkSource()
        : FkObject(), cTime(FkTimeUtils::getCurrentTimeUS()),
          eTime(FkTimeUtils::getCurrentTimeUS()) {

}

FkSource::FkSource(const FkSource &o) : FkObject(o), id(o.id),
                                        cTime(FkTimeUtils::getCurrentTimeUS()),
                                        eTime(FkTimeUtils::getCurrentTimeUS()) {

}

FkSource::~FkSource() {

}
