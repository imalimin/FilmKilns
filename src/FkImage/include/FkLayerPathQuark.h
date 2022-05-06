/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-5-6 13:30:28
*/

#ifndef FK_IMAGE_FKLAYERPATHQUARK_H
#define FK_IMAGE_FKLAYERPATHQUARK_H

#include "FkQuark.h"

FK_SUPER_CLASS(FkLayerPathQuark, FkQuark) {
FK_DEF_CLASS_TYPE_FUNC(FkLayerPathQuark)

public:
    FkLayerPathQuark();

    FkLayerPathQuark(const FkLayerPathQuark &o) = delete;

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    virtual ~FkLayerPathQuark();

private:
    FkResult _onDrawPath(std::shared_ptr<FkProtocol> &p);
};

#endif //FK_IMAGE_FKLAYERPATHQUARK_H