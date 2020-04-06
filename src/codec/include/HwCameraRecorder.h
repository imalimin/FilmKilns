/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_HWCAMERARECORDER_H
#define HWVC_ANDROID_HWCAMERARECORDER_H

#include "AlAbsProcessor.h"
#include "HwWindow.h"
#include "HwAbsFilter.h"
#include "HwSampleFormat.h"
#include "HwCameraInput.h"
#include "AlEgl.h"

/**
 *    Controller                Unit                  Unit               Unit                 Unit
 *[HwCameraRecorder]       [HwCameraInput]          [HwRender]         [HwScreen]         [HwVideoOutput]
 *        |                     |                      |                  |                    |
 *        |>>>>>>>>>>>prepare > |>>>>>>>>>>>>prepare > |>>>>>>>>prepare > |>>>>>>>>>>prepare > |
 *        |                     |                      |                  |                    |
 *        |>>>>>>>>>>>>>>>>>>>>>>>>>>> render filter > |                  |                    |
 *        |                     |                      |                  |                    |
 *        |                     |                      |>>> show screen > |                    |
 *        |                     |                      |                  |                    |
 *        |                     |                      |>>>>>>>>>>>>>>>> notify ready pixels > |
 *        |                     |                      |                  |                    |
 *        |                     |                      | < response ready pixels <<<<<<<<<<<<<<|
 *        |                     |                      |                  |                    |
 *        |                     |                      |>>>>>>>>>>>>>>>>>>>>>>>> send pixels > |
 *        |                     |                      |                  |                    |
 *        |>>>> start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> start > |
 *        |                     |                      |                  |                    |
 *        |>>>> pause >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> pause > |
 *        |                     |                      |                  |                    |
 *
 */
class HwCameraRecorder : public AlAbsProcessor {
public:
    typedef function<void(int32_t)> OnNativeReadyListener;

public:
    HwCameraRecorder();

    virtual ~HwCameraRecorder();

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

    void setFormat(int width, int height, HwSampleFormat format);

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

private:
    const string ALIAS_OF_MIC = "MIC";
    const string ALIAS_OF_CAMERA = "CAMERA";
    const string ALIAS_OF_RENDER = "RENDER";
    const string ALIAS_OF_SCREEN = "SCREEN";
    const string ALIAS_OF_COMPILER = "COMPILER";
    uint32_t oesTex = GL_NONE;
    AlEgl *aSharedContext = nullptr;
    OnNativeReadyListener onNativeReadyListener = nullptr;
    function<void(int64_t)> onRecordListener;
};


#endif //HWVC_ANDROID_HWCAMERARECORDER_H
