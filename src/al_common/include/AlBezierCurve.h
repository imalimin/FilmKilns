/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALBEZIERCURVE_H
#define HWVC_ANDROID_ALBEZIERCURVE_H

#include "Object.h"
#include "AlVec2.h"
#include <vector>

al_class AlBezierCurve al_extend Object {
public:
    static AlBezierCurve *createEmpty();

    static AlBezierCurve *create(const AlVec2 &p0, const AlVec2 &p1, const AlVec2 &p2);

public:
    AlBezierCurve(const AlBezierCurve &o);

    ~AlBezierCurve();

    void getPath(vector<float> &path);

    bool empty();

private:
    AlBezierCurve(const AlVec2 &p0, const AlVec2 &p1, const AlVec2 &p2);

    float _calculateLength();

    void _bezier(float paintSize, std::vector<float> &points);

private:
    AlVec2 p0, p1, p2;
    float a = 0.0F, b = 0.0F, c = 0.0F;
    float _length = 0.0F;
};


#endif //HWVC_ANDROID_ALBEZIERCURVE_H
