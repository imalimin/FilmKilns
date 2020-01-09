/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMMOSAICACTION_H
#define HWVC_ANDROID_ALMMOSAICACTION_H

#include "AlAbsMFilterAction.h"

al_class_ex(AlMMosaicAction, AlAbsMFilterAction) {
public:
    AlMMosaicAction();

    ~AlMMosaicAction();

    void addPoint(const AlVec2 &pointF);

    std::vector<AlVec2> *getPath();

    virtual HwResult draw(HwAbsTexture *src, HwAbsTexture *dest) override;

    virtual HwResult fromElement(AlElement *element) override;

    virtual HwResult toElement(AlElement **element) override;

private:
    AlMMosaicAction(const AlMMosaicAction &o) : AlAbsMFilterAction(o) {};

private:
    std::vector<AlVec2> path;
};


#endif //HWVC_ANDROID_ALMMOSAICACTION_H
