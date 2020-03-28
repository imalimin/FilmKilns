/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlAbsGraph.h"

AlAbsGraph::AlAbsGraph(const std::string alias) : Unit(alias) {

}

AlAbsGraph::~AlAbsGraph() {
    for (auto *it : units) {
        delete it;
    }
    units.clear();
}

bool AlAbsGraph::dispatch(AlMessage *msg) {
    bool ret = Unit::dispatch(msg);
    for (auto *it : units) {
        it->dispatch(msg);
    }
    return ret;
}

bool AlAbsGraph::onCreate(AlMessage *msg) {
    return false;
}

bool AlAbsGraph::onDestroy(AlMessage *msg) {
    return true;
}

void AlAbsGraph::registerAnUnit(Unit *unit) {
    if (unit) {
        unit->setController(this);
        units.emplace_back(unit);
    }
}

void AlAbsGraph::postMessage(AlMessage *msg) {
    Unit::postEvent(msg);
}
