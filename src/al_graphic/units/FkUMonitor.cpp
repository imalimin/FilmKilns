//
// Created by mingyi.li on 2020-11-03.
//

#include "FkUMonitor.h"

FkUMonitor::FkUMonitor(string alias, AlUnitSetting setting) : Unit(alias, setting) {

}

FkUMonitor::~FkUMonitor() {

}

bool FkUMonitor::onCreate(AlMessage *msg) {
    return true;
}

bool FkUMonitor::onDestroy(AlMessage *msg) {
    return true;
}

bool FkUMonitor::onMsgReceived(AlMessage *msg) {
    return true;
}