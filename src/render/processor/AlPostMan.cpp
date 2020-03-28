/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlPostMan.h"
#include "AlRunnable.h"
#include "Unit.h"
#include "AlMessage.h"

#define TAG "AlPostMan"

AlPostMan *AlPostMan::create(const string &name) {
    return new AlPostMan(name);
}

AlPostMan::AlPostMan(const string &name) : UnitPipeline(name) {
}

AlPostMan::~AlPostMan() {
}

void AlPostMan::postMessage(AlMessage *msg) {
    postEvent(msg);
}

int AlPostMan::registerAnUnit(Unit *unit) {
    unit->setController(this);
    return UnitPipeline::registerAnUnit(unit);
}
