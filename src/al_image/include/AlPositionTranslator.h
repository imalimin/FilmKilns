/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPOSITIONTRANSLATOR_H
#define HWVC_ANDROID_ALPOSITIONTRANSLATOR_H

#include "Object.h"
#include "AlSize.h"

al_class(AlPositionTranslator) {
public:
    static void translate(AlSize src, AlSize target, float &x, float &y);

private:
    AlPositionTranslator() : Object() {};

    ~AlPositionTranslator() {};

    AlPositionTranslator(const AlPositionTranslator &o) : Object() {};

};


#endif //HWVC_ANDROID_ALPOSITIONTRANSLATOR_H
