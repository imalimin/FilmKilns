/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-06-20 21:24:53
*/

#include "FkEngineMonitor.h"
#include "FkThread.h"
#include "FkTimeUtils.h"

FK_IMPL_CLASS_TYPE(FkAbsEngineMonitor, FkObject)
FK_IMPL_CLASS_TYPE(FkEngineMonitor, FkAbsEngineMonitor)

FkEngineMonitor::FkEngineMonitor() : FkAbsEngineMonitor() {
}

FkEngineMonitor::FkEngineMonitor(const FkEngineMonitor &o) : FkAbsEngineMonitor(o) {

}

FkEngineMonitor::~FkEngineMonitor() {

}

void FkEngineMonitor::addGraphNode(FkAbsEngineMonitor::GraphNode &node) {

}

void FkEngineMonitor::actionStart(FkAbsEngineMonitor::Action &action) {
    auto time = FkTimeUtils::getCurrentTimeUS();
    std::lock_guard<std::mutex> guard(actMtx);
    auto threadId = FkThread::currentThreadId();
    auto node = std::make_shared<Action>(action);
    node->startTimeInUS = time;
    auto itr = actMap.find(threadId);
    if (itr == actMap.end()) {
        ThreadData data;
        actMap.emplace(std::make_pair(threadId, data));
    }
    itr = actMap.find(threadId);
    ThreadData &data = itr->second;
    if (data.actStack.empty()) {
        data.actions.emplace_back(node);
    } else{
        data.actStack.top()->child.emplace_back(node);
    }
    data.actStack.push(node);
}

void FkEngineMonitor::actionEnd() {
    auto time = FkTimeUtils::getCurrentTimeUS();
    std::lock_guard<std::mutex> guard(actMtx);
    auto threadId = FkThread::currentThreadId();
    auto itr = actMap.find(threadId);
    FkAssert(itr != actMap.end(),);
    ThreadData &data = itr->second;
    data.actStack.top()->endTimeInUS = time;
    data.actStack.pop();
}

std::string FkEngineMonitor::toString() {
    FkString str;
    for (auto &itr: actMap) {
        str.append("thread=");
        str.append(itr.first);
        str.append(", ");
        for (auto &it: itr.second.actions) {
            str.append(it->protoName);
            str.append(":[");
            appendActionStr(str, it);
            str.append("], ");
        }
    }
    return str.toString();
}

void FkEngineMonitor::appendActionStr(FkString &str, std::shared_ptr<Action> &action) {
    str.append("{");
    str.append(action->nodeId);
    str.append(",");
    str.append(action->nodeName);
    str.append(": [");
    for (auto &it: action->child) {
        appendActionStr(str, it);
    }
    str.append("]");
    str.append("}, ");
}
