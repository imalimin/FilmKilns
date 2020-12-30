/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKHELLOQUARK_H
#define FK_FRAMEWORK_FKHELLOQUARK_H

#include "FkQuark.h"
#include <vector>

FK_CLASS FkHelloQuark FK_EXTEND FkQuark {
public:
    FkHelloQuark();

    virtual ~FkHelloQuark();

    virtual void describeProtocols(std::shared_ptr<FkProtDesc> prot) override;

protected:
    int _onHello(FkProtocol *p);

private:
    FkHelloQuark(const FkHelloQuark &o) : FkQuark() {};
};


#endif //HWVC_ANDROID_FKHELLOQUARK_H
