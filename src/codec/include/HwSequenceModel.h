/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWSEQUENCEMODEL_H
#define HWVC_ANDROID_HWSEQUENCEMODEL_H

#include "Object.h"
#include "HwCodecConfig.h"

class HwSequenceModel : public Object {
public:
    static HwSequenceModel *build();

public:
    virtual ~HwSequenceModel();

    HwCodecConfig *getCodecConfig();

private:
    HwSequenceModel();

private:
    HwCodecConfig *pCodecConfig = nullptr;
};


#endif //HWVC_ANDROID_HWSEQUENCEMODEL_H
