//
// Created by mingyi.li on 2019-11-27.
//

#ifndef HWVC_ANDROID_ALMATRIX_H
#define HWVC_ANDROID_ALMATRIX_H

#include "Object.h"

al_class(AlMatrix) {
public:
    AlMatrix();

    AlMatrix(const AlMatrix &o);

    virtual ~AlMatrix();
};


#endif //HWVC_ANDROID_ALMATRIX_H
