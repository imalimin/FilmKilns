/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "../include/HwAbsFilter.h"
#include "../include/HwFBObject.h"

HwAbsFilter::HwAbsFilter() : Object() {
    name = __func__;
}

HwAbsFilter::~HwAbsFilter() {
    if (fbo) {
        delete fbo;
        fbo = nullptr;
    }
    if (program) {
        delete program;
        program = nullptr;
    }
}

bool HwAbsFilter::init() {
    if (initialized)
        return false;
    fbo = HwFBObject::alloc();
    initialized = true;
    return true;
}

void HwAbsFilter::draw(HwAbsTexture *src, HwAbsTexture *dest) {
    if (!fbo || !src || !dest)
        return;
    if (fbo) {
        fbo->bindTex(dest);
        fbo->bind();
    }
    program->draw(src);
    if (fbo) {
        fbo->unbind();
    }
}

void HwAbsFilter::setParams(int *params) {
    if (nullptr == params) return;
    int key = Hw_FILTER_NONE;
    for (int i = 0;; ++i) {
        if (0 == i % 2) {
            key = params[i];
            if (Hw_FILTER_NONE == params[i]) return;
        } else {
            setParam(key, params[i]);
        }
    }
}

void HwAbsFilter::setParam(int key, int value) {

}