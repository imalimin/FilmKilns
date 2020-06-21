/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALUTIMELINE_H
#define HWVC_ANDROID_ALUTIMELINE_H

#include "Unit.h"
#include "AlEventPipeline.h"

al_def_unit(AlUTimeline, Unit) {
public:
    AlUTimeline(const std::string alias);

    virtual ~AlUTimeline();

    virtual bool onCreate(AlMessage *msg) override;

    virtual bool onDestroy(AlMessage *msg) override;

private:
    /// 设置心跳时间间隔
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | hz in US  | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    bool _onSetHzInUS(AlMessage *msg);
    /// 设置时间轴长度
    /// FORMAT:
    /// +-------------------------------------------+
    /// | msg::obj     | msg::arg1 |    msg::arg2   |
    /// +-------------------------------------------+
    /// | none         |           | duration in US |
    /// +-------------------------------------------+
    /// \param msg
    /// \return
    bool _onSetDurationUS(AlMessage *msg);

    void _heartbeat();

private:
    std::shared_ptr<AlEventPipeline> pipe;
    int32_t hzInUS = 0;
    int64_t mDurationInUS = 0;
    int64_t mCurTimeInUS = 0;
};


#endif //HWVC_ANDROID_ALUTIMELINE_H
