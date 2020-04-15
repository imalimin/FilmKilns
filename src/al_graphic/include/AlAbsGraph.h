/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALABSGRAPH_H
#define HWVC_ANDROID_ALABSGRAPH_H

#include "Unit.h"

al_def_unit(AlAbsGraph, Unit), AlAbsPoster {
public:
    AlAbsGraph(const std::string alias);

    virtual ~AlAbsGraph();

    virtual bool onCreate(AlMessage *msg) override;

    virtual bool onDestroy(AlMessage *msg) override;

    void registerAnUnit(Unit *unit);

    void postMessage(AlMessage *msg) override;

protected:
    virtual bool dispatch(AlMessage *msg) override;

private:
    std::vector<Unit *> units;
};


#endif //HWVC_ANDROID_ALABSGRAPH_H
