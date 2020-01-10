/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPOINTPROGRAM_H
#define HWVC_ANDROID_ALPOINTPROGRAM_H

#include "AlAbsGLProgram.h"
#include "HwAbsTexture.h"
#include "AlVec2.h"
#include "vector"

al_class_ex(AlPointProgram, AlAbsGLProgram) {
public:
    static AlPointProgram *create(std::string *v, std::string *f);

public:
    virtual ~AlPointProgram();

    virtual void draw(HwAbsTexture *tex) override;

    void updatePosition(std::vector<AlVec2 *> &position);

private:
    AlPointProgram(std::string *v, std::string *f);

    AlPointProgram(const AlPointProgram &o) : AlAbsGLProgram(o) {};

private:
    int32_t aPosLoc = -1;
    int32_t uTexLoc = -1;

    float *positions = nullptr;
    size_t posCount = 0;
};


#endif //HWVC_ANDROID_ALPOINTPROGRAM_H
