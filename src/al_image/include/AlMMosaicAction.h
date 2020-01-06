/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALMMOSAICACTION_H
#define HWVC_ANDROID_ALMMOSAICACTION_H

#include "AlAbsMAction.h"

al_class_ex(AlMMosaicAction, AlAbsMAction) {
public:
    AlMMosaicAction();

    ~AlMMosaicAction();

    void addPoint(const AlPointF &pointF);

    std::vector<AlPointF> *getPath();

    HwResult measure(AlImgLayerDescription &layer, AlImageLayerDrawModel *description) override;

    virtual HwResult fromElement(AlElement *element) override;

    virtual HwResult toElement(AlElement **element) override;

private:
    AlMMosaicAction(const AlMMosaicAction &o) : AlAbsMAction(o) {};

private:
    std::vector<AlPointF> points;
};


#endif //HWVC_ANDROID_ALMMOSAICACTION_H
