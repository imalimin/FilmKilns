/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwAbsProcessor.h"

HwAbsProcessor::HwAbsProcessor(string name) : Object() {
    pipeline = new UnitPipeline("VideoProcessor");
}

HwAbsProcessor::~HwAbsProcessor() {

}