/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HARDWAREVIDEOCODEC_UNIT_H
#define HARDWAREVIDEOCODEC_UNIT_H

#define KID(a, b, c, d) ((d) | ((c) << 8) | ((b) << 16) | ((unsigned)(a) << 24))

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
static constexpr int EVENT_SCREEN_DRAW_TEX = KID('S', 'C', 'R', 0x02);
static constexpr int EVENT_SCREEN_UPDATE_WINDOW = KID('S', 'C', 'R', 0x03);
static constexpr int EVENT_SCREEN_UPDATE_NOTIFY = KID('S', 'C', 'R', 0x04);

static constexpr int EVENT_VIDEO_START = KID('V', 'D', 'O', 0x01);
static constexpr int EVENT_VIDEO_PAUSE = KID('V', 'D', 'O', 0x02);
static constexpr int EVENT_VIDEO_SEEK = KID('V', 'D', 'O', 0x03);
static constexpr int EVENT_VIDEO_SET_SOURCE = KID('V', 'D', 'O', 0x04);
static constexpr int EVENT_VIDEO_LOOP = KID('V', 'D', 'O', 0x05);
static constexpr int EVENT_VIDEO_STOP = KID('V', 'D', 'O', 0x06);
static constexpr int MSG_VIDEO_PROGRESS = KID('V', 'D', 'O', 0x07);

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
static constexpr int MSG_MICROPHONE_FORMAT = KID('M', 'I', 'C', 0x03);

static constexpr int EVENT_CAMERA_INVALIDATE = KID('C', 'A', 'M', 0x00);
static constexpr int MSG_CAMERA_UPDATE_SIZE = KID('C', 'A', 'M', 0x01);
static constexpr int MSG_CAMERA_OES_TEX_NOTIFY = KID('C', 'A', 'M', 0x02);
static constexpr int MSG_CAMERA_RUN = KID('C', 'A', 'M', 0x03);
static constexpr int MSG_TIMESTAMP = KID('C', 'A', 'M', 0x04);

static constexpr int MSG_VIDEO_COMPILER_BACKWARD = KID('H', 'V', 'C', 0x00);
static constexpr int MSG_VIDEO_OUTPUT_PATH = KID('H', 'V', 'C', 0x01);
static constexpr int MSG_VIDEO_OUTPUT_SIZE = KID('H', 'V', 'C', 0x02);
static constexpr int MSG_VIDEO_COMPILER_TIME = KID('H', 'V', 'C', 0x03);
static constexpr int MSG_VIDEO_OUTPUT_BITRATE_LEVEL = KID('H', 'V', 'C', 0x04);
static constexpr int MSG_VIDEO_OUTPUT_PROFILE = KID('H', 'V', 'C', 0x05);
static constexpr int MSG_VIDEO_OUTPUT_PRESET = KID('H', 'V', 'C', 0x06);
static constexpr int MSG_VIDEO_OUTPUT_MAX_SIZE = KID('H', 'V', 'C', 0x07);
static constexpr int MSG_VIDEO_OUTPUT_ENABLE_HARD = KID('H', 'V', 'C', 0x08);

static constexpr int EVENT_LAYER_ADD = KID('A', 'I', 'G', 0x00);
static constexpr int EVENT_LAYER_IMPORT = KID('A', 'I', 'G', 0x01);
static constexpr int EVENT_AIMAGE_REDO = KID('A', 'I', 'G', 0x02);
static constexpr int EVENT_AIMAGE_UNDO = KID('A', 'I', 'G', 0x03);
static constexpr int EVENT_LAYER_PAINT = KID('A', 'I', 'G', 0x04);
static constexpr int EVENT_LAYER_SCALE = KID('A', 'I', 'G', 0x05);
static constexpr int EVENT_LAYER_SCALE_POST = KID('A', 'I', 'G', 0x06);
static constexpr int EVENT_LAYER_ROTATE = KID('A', 'I', 'G', 0x07);
static constexpr int EVENT_LAYER_ROTATE_POST = KID('A', 'I', 'G', 0x08);
static constexpr int EVENT_LAYER_TRANS = KID('A', 'I', 'G', 0x09);
static constexpr int EVENT_LAYER_TRANS_POST = KID('A', 'I', 'G', 0xa);
static constexpr int EVENT_LAYER_QUERY_ID = KID('A', 'I', 'G', 0xb);
static constexpr int EVENT_LAYER_QUERY_ID_NOTIFY = KID('A', 'I', 'G', 0xc);
static constexpr int EVENT_LAYER_REMOVE = KID('A', 'I', 'G', 0xe);
static constexpr int EVENT_LAYER_MOVE = KID('A', 'I', 'G', 0x10);
static constexpr int EVENT_LAYER_ALPHA = KID('A', 'I', 'G', 0x11);
static constexpr int EVENT_LAYER_CROP = KID('A', 'I', 'G', 0x12);
static constexpr int EVENT_LAYER_CROP_CANCEL = KID('A', 'I', 'G', 0x13);
static constexpr int EVENT_LAYER_ALIGN_CROP = KID('A', 'I', 'G', 0x14);
static constexpr int EVENT_LAYER_ALIGN_CROP_CANCEL = KID('A', 'I', 'G', 0x15);
static constexpr int EVENT_LAYER_EXPORT = KID('A', 'I', 'G', 0x16);
static constexpr int EVENT_LAYER_EXPORT_FINISH = KID('A', 'I', 'G', 0x17);
static constexpr int EVENT_LAYER_IMPORT_FINISH = KID('A', 'I', 'G', 0x18);
static constexpr int EVENT_LAYER_QUERY_INFO = KID('A', 'I', 'G', 0x1a);
static constexpr int EVENT_LAYER_QUERY_INFO_NOTIFY = KID('A', 'I', 'G', 0x1b);
static constexpr int MSG_LAYER_ADD_EMPTY = KID('A', 'I', 'G', 0x1c);
static constexpr int MSG_LAYER_RESIZE_CANVAS = KID('A', 'I', 'G', 0x1d);
static constexpr int MSG_LAYER_PAINT_POINT = KID('A', 'I', 'G', 0x1e);

static constexpr int EVENT_LAYER_MEASURE = KID('A', 'L', 'M', 0x00);
static constexpr int EVENT_LAYER_MEASURE_CANVAS_NOTIFY = KID('A', 'L', 'M', 0x01);

static constexpr int EVENT_CANVAS_RESIZE = KID('A', 'L', 'R', 0x00);
static constexpr int EVENT_LAYER_RENDER_CLEAR = KID('A', 'L', 'R', 0x01);
static constexpr int EVENT_LAYER_RENDER_DRAW = KID('A', 'L', 'R', 0x02);
static constexpr int EVENT_LAYER_RENDER_SHOW = KID('A', 'L', 'R', 0x03);
static constexpr int EVENT_CANVAS_SAVE = KID('A', 'L', 'R', 0x04);
static constexpr int EVENT_CANVAS_CROP = KID('A', 'L', 'R', 0x05);
static constexpr int EVENT_CANVAS_CLEAR_DONE = KID('A', 'L', 'R', 0x06);
static constexpr int EVENT_CANVAS_DRAW_DONE = KID('A', 'L', 'R', 0x07);
static constexpr int EVENT_CANVAS_SAVE_FINISH = KID('A', 'L', 'R', 0x08);
static constexpr int MSG_CANVAS_SET_BG = KID('A', 'L', 'R', 0x09);

static constexpr int EVENT_LAYER_FILTER_RENDER = KID('A', 'L', 'F', 0x00);
static constexpr int EVENT_LAYER_REMOVE_CACHE_LAYER = KID('A', 'L', 'F', 0x01);

static constexpr int EVENT_IMAGE_CODEC_DECODE = KID('I', 'C', 'D', 0x00);
static constexpr int EVENT_IMAGE_CODEC_DECODE_NOTIFY = KID('I', 'C', 'D', 0x01);
static constexpr int EVENT_IMAGE_CODEC_ENCODE = KID('I', 'C', 'D', 0x02);
static constexpr int EVENT_IMAGE_CODEC_ENCODE_NOTIFY = KID('I', 'C', 'D', 0x03);

static constexpr int MSG_TEX_READER_REQ_PIXELS = KID('A', 'T', 'R', 0x01);
static constexpr int MSG_TEX_READER_NOTIFY_PIXELS = KID('A', 'T', 'R', 0x02);

static constexpr int MSG_TIMELINE_HEARTBEAT = KID('A', 'T', 'L', 0x01);
static constexpr int MSG_TIMELINE_SET_HZ_IN_US = KID('A', 'T', 'L', 0x02);
static constexpr int MSG_TIMELINE_SET_DURATION = KID('A', 'T', 'L', 0x03);
static constexpr int MSG_TIMELINE_PROGRESS_NOTIFY = KID('A', 'T', 'L', 0x04);
static constexpr int MSG_TIMELINE_START = KID('A', 'T', 'L', 0x05);
static constexpr int MSG_TIMELINE_PAUSE = KID('A', 'T', 'L', 0x06);
static constexpr int MSG_TIMELINE_BEGIN = KID('A', 'T', 'L', 0x07);
static constexpr int MSG_TIMELINE_END = KID('A', 'T', 'L', 0x08);
static constexpr int MSG_TIMELINE_SEEK = KID('A', 'T', 'L', 0x09);

static constexpr int MSG_SEQUENCE_TRACK_ADD = KID('A', 'S', 'Q', 0x01);
static constexpr int MSG_SEQUENCE_TRACK_ADD_DONE = KID('A', 'S', 'Q', 0x02);
static constexpr int MSG_SEQUENCE_TRACK_REMOVE = KID('A', 'S', 'Q', 0x03);
static constexpr int MSG_SEQUENCE_TRACK_REMOVE_DONE = KID('A', 'S', 'Q', 0x04);
static constexpr int MSG_SEQUENCE_BEAT_AUDIO = KID('A', 'S', 'Q', 0x05);
static constexpr int MSG_SEQUENCE_BEAT_VIDEO = KID('A', 'S', 'Q', 0x06);

static constexpr int MSG_AUDIOS_TRACK_ADD = KID('A', 'A', 'S', 0x01);
static constexpr int MSG_AUDIOS_TRACK_ADD_DONE = KID('A', 'A', 'S', 0x02);
static constexpr int MSG_AUDIOS_END = KID('A', 'A', 'S', 0x03);
static constexpr int MSG_AUDIOS_TRACK_REMOVE = KID('A', 'A', 'S', 0x04);

static constexpr int MSG_VIDEOS_ADD = KID('A', 'V', 'S', 0x01);

#include "Object.h"
#include "UnitPipeline.h"
#include "AlMessage.h"
#include "HwBundle.h"
#include "AlPostMan.h"
#include <map>

#define al_def_unit(cls, parent) \
AL_CLASS cls AL_EXTEND parent \

#define al_def_msg(name) \
bool name(AlMessage *m); \

#define al_reg_msg(id, func) \
registerEvent(id, reinterpret_cast<EventFunc>(&func)); \

using namespace std;

class HwModelProvider;

class AlAbsGraph;

typedef bool (Unit::*EventFunc)(AlMessage *);

class Event : public Object {
public:
    Event(int what, EventFunc handler);

    virtual ~Event();

    bool handle(Unit *unit, AlMessage *msg);

protected:
    int what = 0;
    EventFunc handler;
};

class Unit : public Object {
private:
    friend class UnitPipeline;

    friend class AlAbsGraph;

public:
    al_class AlUnitSetting al_extend Object {
    public:
        AlUnitSetting(bool hosted = false);

        AlUnitSetting(const AlUnitSetting &o);

        virtual ~AlUnitSetting();

    public:
        /// false: This unit will be delete when UnitPipeline delete.
        /// true: This unit will not be delete when UnitPipeline delete.
        bool hosted = false;
    };

public:
    /**
     * @param Alias is IMPORTANT for an unit. It is a tag of model provider.
     */
    Unit(string alias, AlUnitSetting setting = AlUnitSetting());

    virtual ~Unit();

    virtual void setController(AlAbsPoster *poster);

    bool registerEvent(int what, EventFunc handler);

    virtual bool onCreate(AlMessage *msg) = 0;

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
    virtual bool onDestroy(AlMessage *msg) = 0;

    /** Model Provider START */
    void setModelProvider(HwModelProvider *provider);

    HwModelProvider *getModelProvider();

    virtual const int32_t getInt32(string key);

    virtual const int64_t getInt64(string key);

    virtual const string getString(string key);

    virtual Object *getObject(string key);
    /** Model Provider END */

protected:
    /// 广播分发接收函数，通常由UnitPipeline调用
    /// Don`t call this func.
    /// \param msg 事件消息
    /// \return true:我可以处理这个事件，false:无法处理这个事件
    virtual bool dispatch(AlMessage *msg);

    /// 广播消息
    /// @deprecated Install by postMessage.
    /// \param msg
    void postEvent(AlMessage *msg);

    /// 广播消息
    /// \param msg
    void postMessage(AlMessage *msg);

private:
    Unit(const Unit &o) : Object() {};

private:
    string alias;
    AlUnitSetting setting;
    map<int, Event *> eventMap;
    AlAbsPoster *poster = nullptr;
    HwModelProvider *provider = nullptr;
    bool created = false;
};

class HwModelProvider : public Unit {
public:
    HwModelProvider(string alias);

    virtual ~HwModelProvider();

    bool onCreate(AlMessage *msg) override;

    bool onDestroy(AlMessage *msg) override;

    bool eventPutInt32(AlMessage *msg);

    bool eventPutInt64(AlMessage *msg);

    bool eventPutString(AlMessage *msg);

    bool eventPutObject(AlMessage *msg);

    const int32_t getInt32(string key) override;

    const int64_t getInt64(string key) override;

    const string getString(string key) override;

    Object *getObject(string key) override;

public:
    static const int EVENT_PUT_INT32;
    static const int EVENT_PUT_INT64;
    static const int EVENT_PUT_STRING;
    static const int EVENT_PUT_OBJECT;

private:
    HwBundle bundle;
};


#endif //HARDWAREVIDEOCODEC_UNIT_H
