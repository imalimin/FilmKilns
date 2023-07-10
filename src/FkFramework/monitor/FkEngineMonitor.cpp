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
    FkString str;
    std::ofstream stream;
    stream.open(outputPath.c_str(), std::ios::in | std::ios::trunc);
    for (auto &itr: actMap) {
        for (auto &it: itr.second.actions) {
            stream << "Thread_" << itr.first << "     " << 1.0f << ": cycles: " << std::endl;
            str.append("thread=");
            str.append(itr.first);
            str.append(", ");
            stream << appendAction(stream, it) << std::endl;
            stream << "\t" << it->protoId << " " << it->protoName << "(undefine)" << std::endl << std::endl;
        }
    }
    stream.flush();
    stream.close();
    return str.toString();
}

std::string FkEngineMonitor::appendAction(std::ofstream &stream, std::shared_ptr<Action> &action) {
    for (auto &it: action->child) {
        auto tmp = appendAction(stream, it);
        stream << tmp << std::endl;
    }
    std::string str = "";
    str += "\t";
    str += FkString::valueOf(action->nodeId);
    str += " ";
    str += action->nodeName;
    str += "(undefine)";
    return str;
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
