/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWVIDEOOUTPUT_H
#define HWVC_ANDROID_HWVIDEOOUTPUT_H

#include "Unit.h"
#include "HwAbsVideoEncoder.h"
#include "HwVideoFrame.h"
#include "HwAudioFrame.h"
#include "AlSize.h"
#include "AlBuffer.h"
#include <atomic>
#include <list>
#include <vector>

al_class_ex(AlVideoCompiler, Unit) {
public:
    AlVideoCompiler(string alias);

    virtual ~AlVideoCompiler();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    void setRecordListener(function<void(int64_t)> listener);

private:
    /// 输出路径
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | none         | none      | none      |   path    |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool _onSetOutPath(AlMessage *msg);

    /// 编码大小
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | size         | none      | none      |           |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool _onSetSize(AlMessage *msg);

    /// 音频格式
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | format       | none      | none      |           |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool _onFormat(AlMessage *msg);

    /// 时间戳
    /// FORMAT:
    /// +--------------------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 | msg::desc |
    /// +--------------------------------------------------+
    /// | none         | none      | time      |           |
    /// +--------------------------------------------------+
    /// \param msg
    /// \return
    bool _onTimestamp(AlMessage *msg);

    bool _onDrawDone(AlMessage *msg);

    bool _onSamples(AlMessage *msg);

    bool _onWrite(AlMessage *msg);

    bool _onStart(AlMessage *msg);

    bool _onPause(AlMessage *msg);

    bool _onBackward(AlMessage *msg);

    void _write(AlBuffer *buf, int64_t tsInNs);

    int64_t getRecordTimeInUs();

    void _initialize();

private:
    AlSize size;
    std::string path;
    HwSampleFormat aFormat;
    HwAbsVideoEncoder *encoder = nullptr;
    HwVideoFrame *videoFrame = nullptr;
    HwAudioFrame *audioFrame = nullptr;
    std::atomic_bool recording;
    // last timestamp in ns.
    int64_t lastTsInNs = -1, lastATsInNs = -1;
    // frame timestamp in ns.
    int64_t vTimestamp = -1, aTimestamp = -1;
    int64_t offsetOfDuration = 0;
    int count = 0;
    /**
     * Listeners
     */
    function<void(int64_t)> recordListener = nullptr;
    bool initialized = false;
    std::deque<int64_t> mPtsQueue;
};


#endif //HWVC_ANDROID_HWVIDEOOUTPUT_H
