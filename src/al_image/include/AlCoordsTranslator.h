/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALCOORDSTRANSLATOR_H
#define HWVC_ANDROID_ALCOORDSTRANSLATOR_H

#include "Object.h"
#include "AlSize.h"
#include "AlImageLayerModel.h"

al_class(AlCoordsTranslator) {
public:
    static void translate(AlSize src, AlSize target, float &x, float &y);

    static void changeCanvasStayLoc(AlSize *src, AlSize *dest,
                                    AlPointF *anchor, AlImageLayerModel *layer);

private:
    AlCoordsTranslator() : Object() {};

    ~AlCoordsTranslator() {};

    AlCoordsTranslator(const AlCoordsTranslator &o) : Object() {};

};


#endif //HWVC_ANDROID_ALCOORDSTRANSLATOR_H
