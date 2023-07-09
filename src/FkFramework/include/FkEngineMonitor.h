/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-06-20 21:24:53
*/

#ifndef FK_FRAMEWORK_FKENGINEMONITOR_H
#define FK_FRAMEWORK_FKENGINEMONITOR_H

#include "FkObject.h"
#include "FkString.h"
#include <unordered_map>
#include <stack>
#include <vector>
#include <mutex>

FK_SUPER_CLASS(FkAbsEngineMonitor, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkAbsEngineMonitor)

public:
    struct GraphNode {
        long id;
        long preId;
        const std::string name;
    };
    struct Action {
        long nodeId;
        const char *nodeName;
        long protoId;
        const char *protoName;
        int64_t startTimeInUS;
        int64_t endTimeInUS;
        std::vector<std::shared_ptr<Action>> child;
    };
    struct ThreadData {
        std::vector<std::shared_ptr<Action>> actions;
        std::stack<std::shared_ptr<Action>> actStack;
    };
    virtual void addGraphNode(GraphNode &node) = 0;
    virtual void actionStart(Action &action) = 0;
    virtual void actionEnd() = 0;
};

FK_SUPER_CLASS(FkEngineMonitor, FkAbsEngineMonitor) {
FK_DEF_CLASS_TYPE_FUNC(FkEngineMonitor)

public:
    FkEngineMonitor();

    FkEngineMonitor(const FkEngineMonitor &o);

    virtual ~FkEngineMonitor();

    virtual void addGraphNode(GraphNode &node) override;

    virtual void actionStart(Action &action) override;

    virtual void actionEnd() override;

    virtual std::string toString() override;

private:
    void appendActionStr(FkString &str, std::shared_ptr<Action> &action);

private:
    std::mutex actMtx;
    std::unordered_map<long, ThreadData> actMap;
};

#endif //FK_FRAMEWORK_FKENGINEMONITOR_H