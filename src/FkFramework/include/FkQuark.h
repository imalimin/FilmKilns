/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKQUARK_H
#define FK_FRAMEWORK_FKQUARK_H

#include "FkObject.h"
#include "FkProtDesc.h"

class FkSession;

FK_ABS_CLASS FkQuark FK_EXTEND FkObject {
    friend FkSession;
public:
    typedef int (FkQuark::*ProtHandler)(std::shared_ptr<FkProtocol>);

public:
    FkQuark();

    virtual ~FkQuark();

    virtual void describeProtocols(std::shared_ptr<FkProtDesc> desc) = 0;

    void onCreate();

    void onDestroy();

    void onStart();

    void onStop();

    int dispatch(std::shared_ptr<FkProtocol> p);

private:
    FkQuark(const FkQuark &o) : FkObject() {};

    FkResult accept(const std::shared_ptr<FkProtocol> p);

private:
    std::shared_ptr<FkProtDesc> prot = nullptr;
};


#endif //FK_FRAMEWORK_FKQUARK_H
