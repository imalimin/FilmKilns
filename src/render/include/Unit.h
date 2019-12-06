/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_UNIT_H
#define HARDWAREVIDEOCODEC_UNIT_H

#include "Object.h"
#include "UnitPipeline.h"
#include "EventPipeline.h"
#include "Message.h"
#include "HwBundle.h"
#include <map>
//#include "HwModelProvider.h"

#define KID(a, b, c, d) ((d) | ((c) << 8) | ((b) << 16) | ((unsigned)(a) << 24))

using namespace std;

static constexpr int EVENT_COMMON_RELEASE = KID('C', 'O', 'M', 0x01);
static constexpr int EVENT_COMMON_PREPARE = KID('C', 'O', 'M', 0x02);
static constexpr int EVENT_COMMON_INVALIDATE = KID('C', 'O', 'M', 0x03);
static constexpr int EVENT_COMMON_START = KID('C', 'O', 'M', 0x04);
static constexpr int EVENT_COMMON_PAUSE = KID('C', 'O', 'M', 0x05);
/**
 * Notify pixels is ready to someone.
 */
static constexpr int EVENT_COMMON_PIXELS_READY = KID('C', 'O', 'M', 0x06);
/**
 * Response 'EVENT_COMMON_PIXELS_READY' event for notify Unit read pixels.
 * If nobody response this event, read pixels action will be cancel.
 */
static constexpr int EVENT_COMMON_PIXELS_READ = KID('C', 'O', 'M', 0x07);
/**
 * If someone response 'EVENT_COMMON_PIXELS_READY' event, then send pixels to pipeline.
 * You can handle this event.
 */
static constexpr int EVENT_COMMON_PIXELS = KID('C', 'O', 'M', 0x08);

static constexpr int EVENT_IMAGE_SHOW = KID('I', 'M', 'G', 0x01);

static constexpr int EVENT_RENDER_FILTER = KID('R', 'N', 'D', 0x01);// 绘制滤镜
static constexpr int EVENT_RENDER_SET_FILTER = KID('R', 'N', 'D', 0x02);// 更新滤镜

static constexpr int EVENT_SCREEN_DRAW = KID('S', 'C', 'R', 0x01);
static constexpr int EVENT_SCREEN_UPDATE_WINDOW = KID('S', 'C', 'R', 0x02);

static constexpr int EVENT_VIDEO_START = KID('V', 'D', 'O', 0x01);
static constexpr int EVENT_VIDEO_PAUSE = KID('V', 'D', 'O', 0x02);
static constexpr int EVENT_VIDEO_SEEK = KID('V', 'D', 'O', 0x03);
static constexpr int EVENT_VIDEO_SET_SOURCE = KID('V', 'D', 'O', 0x04);
static constexpr int EVENT_VIDEO_LOOP = KID('V', 'D', 'O', 0x05);
static constexpr int EVENT_VIDEO_STOP = KID('V', 'D', 'O', 0x06);

static constexpr int EVENT_AUDIO_START = KID('A', 'D', 'O', 0x01);
static constexpr int EVENT_AUDIO_PAUSE = KID('A', 'D', 'O', 0x02);
static constexpr int EVENT_AUDIO_STOP = KID('A', 'D', 'O', 0x03);
static constexpr int EVENT_AUDIO_SEEK = KID('A', 'D', 'O', 0x04);
static constexpr int EVENT_AUDIO_LOOP = KID('A', 'D', 'O', 0x05);

static constexpr int EVENT_SPEAKER_FEED = KID('S', 'P', 'K', 0x01);

static constexpr int EVENT_VIDEO_OUT_START = KID('V', 'O', 'P', 0x01);
static constexpr int EVENT_VIDEO_OUT_PAUSE = KID('V', 'O', 'P', 0x02);

static constexpr int EVENT_MICROPHONE_LOOP = KID('M', 'I', 'C', 0x01);
static constexpr int EVENT_MICROPHONE_OUT_SAMPLES = KID('M', 'I', 'C', 0x02);

static constexpr int EVENT_CAMERA_INVALIDATE = KID('C', 'A', 'M', 0x00);

static constexpr int EVENT_VIDEO_COMPILER_BACKWARD = KID('H', 'V', 'C', 0x00);

static constexpr int EVENT_AIMAGE_UPDATE_LAYER = KID('A', 'I', 'G', 0x00);
static constexpr int EVENT_AIMAGE_UPDATE_CANVAS = KID('A', 'I', 'G', 0x01);
static constexpr int EVENT_AIMAGE_SAVE = KID('A', 'I', 'G', 0x02);

/**
 * Define class HwModelProvider.
 */
class HwModelProvider;

typedef bool (Unit::*EventFunc)(Message *);

class Event : public Object {
public:
    Event(int what, EventFunc handler);

    virtual ~Event();

    bool dispatch(Unit *unit, Message *msg);

protected:
    int what = 0;
    EventFunc handler;
};

class Unit : public Object {
private:
    friend class UnitPipeline;

public:
    /**
     * @param Alias is IMPORTANT for an unit. It is a tag of model provider.
     */
    Unit(string alias);

    virtual ~Unit();

    virtual void setController(UnitPipeline *pipeline);

    bool registerEvent(int what, EventFunc handler);

    virtual bool onCreate(Message *msg) = 0;

    /// 该函数会接收到一个pipeline实例即将销毁的消息
    /// 请在此释放跟opengl相关的资源，或其它
    /// FORMAT:
    /// +--------------------------------------+
    /// | msg::obj     | msg::arg1 | msg::arg2 |
    /// +--------------------------------------+
    /// | none         | none      | none      |
    /// +--------------------------------------+
    /// \param msg
    /// \return
    virtual bool onDestroy(Message *msg) = 0;

    /** Model Provider START */
    void setModelProvider(HwModelProvider *provider);

    HwModelProvider *getModelProvider();

    const int32_t &getInt32(string key);

    const int64_t &getInt64(string key);

    const string &getString(string key);

    Object *getObject(string key);
    /** Model Provider END */

protected:
    void postEvent(Message *msg);

private:
    /**
     * @msg 事件消息
     * @return true:我可以处理这个事件，false:无法处理这个事件
     */
    bool dispatch(Message *msg);

private:
    string alias;
    map<int, Event *> eventMap;
    UnitPipeline *pipeline = nullptr;
    HwModelProvider *provider = nullptr;
};

class HwModelProvider : public Unit {
public:
    HwModelProvider(string alias);

    virtual ~HwModelProvider();

    bool onCreate(Message *msg) override;

    bool onDestroy(Message *msg) override;

    bool eventPutInt32(Message *msg);

    bool eventPutInt64(Message *msg);

    bool eventPutString(Message *msg);

    bool eventPutObject(Message *msg);

    const int32_t &getInt32(string key);

    const int64_t &getInt64(string key);

    const string &getString(string key);

    Object *getObject(string key);

public:
    static const int EVENT_PUT_INT32;
    static const int EVENT_PUT_INT64;
    static const int EVENT_PUT_STRING;
    static const int EVENT_PUT_OBJECT;

private:
    HwBundle bundle;
};


#endif //HARDWAREVIDEOCODEC_UNIT_H
