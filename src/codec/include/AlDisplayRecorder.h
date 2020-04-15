/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALDISPLAYRECORDER_H
#define HWVC_ANDROID_ALDISPLAYRECORDER_H

#include "AlAbsProcessor.h"
#include "HwWindow.h"
#include "HwAbsFilter.h"
#include "HwSampleFormat.h"
#include "AlEgl.h"
#include "AlMatrix.h"

al_class_ex(AlDisplayRecorder, AlAbsProcessor) {
public:
    typedef function<void(int32_t)> OnNativeReadyListener;

public:
    AlDisplayRecorder();

    virtual ~AlDisplayRecorder();

    virtual void onCreate() override;

    virtual void onDestroy() override;


    void updateWindow(HwWindow *win);

    void start();

    void pause();

    ///
    /// \param matrix
    /// \param w 视频宽度
    /// \param h 视频高度
    /// \param tsInNs 视频pts
    void invalidate(AlMatrix *matrix, int64_t tsInNs);

    void setOutputFilePath(string filePath);

    void setBitrate(int32_t bitrate);

    void setProfile(std::string profile);

    void setFormat(int width, int height, HwSampleFormat format);

    void setScaleSize(int width, int height);

    void cropOutputSize(float left, float top, float right, float bottom);

    void setFilter(HwAbsFilter *filter);

    void runOnCameraContext(function<void()> func);

    void setCameraSize(int32_t w, int32_t h);

    /**
     * delete nearly video clip.
     */
    void backward();

    void setRecordListener(function<void(int64_t)> listener);

    void setOnNativeReadyListener(OnNativeReadyListener l);

private:
    bool _onOESTexNotify(AlMessage *msg);

    bool _onRecordProgress(AlMessage *msg);

    bool _onCanvasUpdate(AlMessage *msg);

private:
    const string ALIAS_OF_MIC = "MIC";
    const string ALIAS_OF_CAMERA = "CAMERA";
    const string ALIAS_OF_RENDER = "RENDER";
    const string ALIAS_OF_READER = "READER";
    const string ALIAS_OF_SCREEN = "SCREEN";
    const string ALIAS_OF_COMPILER = "COMPILER";
    uint32_t oesTex = GL_NONE;
    AlEgl *aSharedContext = nullptr;
    OnNativeReadyListener onNativeReadyListener = nullptr;
    function<void(int64_t)> onRecordListener;
};


#endif //HWVC_ANDROID_ALDISPLAYRECORDER_H
