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

#include <fstream>
#include "FkThread.h"
#include "FkTimeUtils.h"

FK_IMPL_CLASS_TYPE(FkAbsEngineMonitor, FkObject)
FK_IMPL_CLASS_TYPE(FkEngineMonitor, FkAbsEngineMonitor)

FkEngineMonitor::FkEngineMonitor(std::string outputPath)
        : FkAbsEngineMonitor(), outputPath(std::move(outputPath)) {
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
    std::ofstream stream;
    stream.open(outputPath.c_str(), std::ios::in | std::ios::trunc);
    for (auto &itr: actMap) {
        FkString head;
        head << "Thread_" << itr.first;
        for (auto &it: itr.second.actions) {
//            if (strcmp(it->protoName, "FkRenderProto") != 0) {
//                continue;
//            }
            FkString str;
            FkString tmp = getActionStr(it->protoId, it->protoName, it);
            str.begin() << tmp;
            tmp = getActionStr(it->nodeId, it->nodeName, it);
            str.begin() << tmp;
            appendAction(head, str, stream, it);
        }
    }
    stream.flush();
    stream.close();
    return "";
}

void FkEngineMonitor::appendAction(const FkString &head, FkString str, std::ofstream &stream, std::shared_ptr<Action> &action) {
    if (action->child.empty()) {
        stream << head.c_str() << "     " << "0/0" << "     " << "[000]" << " " << 1.0f << ": cycles:" << std::endl;
        stream << str.c_str() << std::endl;
        return;
    }
    for (auto &it: action->child) {
        FkString tmp = getActionStr(it->nodeId, it->nodeName, it);
        tmp << str;
        appendAction(head, tmp, stream, it);
    }
}

FkString FkEngineMonitor::getActionStr(long id, const char *name, std::shared_ptr<Action> &action) {
    FkString str;
    str << "\t" << id << " " << name << "/c_" << (action->endTimeInUS - action->startTimeInUS) << " (undefine)\n";
    return str;
}
