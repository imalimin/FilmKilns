/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-03-14 23:34:10
*/

#include "FkDeviceImageCompo.h"

FK_IMPL_CLASS_TYPE(FkDeviceImageCompo, FkComponent)

FkDeviceImageCompo::FkDeviceImageCompo(std::shared_ptr<FkDeviceImage> &deviceImage)
        : FkComponent(),
          deviceImage(deviceImage) {

}

FkDeviceImageCompo::FkDeviceImageCompo(const FkDeviceImageCompo &o) : FkComponent(o),
                                                                      deviceImage(o.deviceImage) {

}

FkDeviceImageCompo::~FkDeviceImageCompo() {
    deviceImage = nullptr;
}