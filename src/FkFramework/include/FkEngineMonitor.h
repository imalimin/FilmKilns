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

FK_SUPER_CLASS(IFkEngineMonitor, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(IFkEngineMonitor)

public:
    struct GraphNode {
        FkID id;
        FkID preId;
        const std::string name;
    };
    struct Action {
        FkID nodeId;
        const std::string name;
        int64_t startTimeInUS;
        int64_t endTimeInUS;
        long threadId;
        const std::string threadName;
    };
    virtual void addGraphNode(GraphNode &node) = 0;
    virtual void recordAction(Action &action) = 0;
};

FK_SUPER_CLASS(FkEngineMonitor, IFkEngineMonitor) {
FK_DEF_CLASS_TYPE_FUNC(FkEngineMonitor)

public:
    FkEngineMonitor();

    FkEngineMonitor(const FkEngineMonitor &o);

    virtual ~FkEngineMonitor();

    virtual void addGraphNode(GraphNode &node) override;

    virtual void recordAction(Action &action) override;
};

#endif //FK_FRAMEWORK_FKENGINEMONITOR_H