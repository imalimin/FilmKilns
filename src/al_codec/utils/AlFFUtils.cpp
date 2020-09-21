/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFFUtils.h"
#include "AlLogcat.h"
#include "platform/android/core/AlFFAndroidDec.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
#include "fftools/ffmpeg.h"
#ifdef __cplusplus
}
#endif

#define TAG "AlFFUtils"

int AlFFUtils::init() {
    std::call_once(onceFlag, []() {
        av_register_all();
        avcodec_register_all();
        al_media_codec_dec_register();
    });
    return 0;
}

int AlFFUtils::avSamplesCopy(AVFrame *dest, AVFrame *src) {
    av_free(dest->data[0]);
    av_samples_alloc(dest->data,
                     &dest->linesize[0],
                     src->channels,
                     src->nb_samples,
                     AVSampleFormat(src->format), 0);
    dest->format = src->format;
    memcpy(dest->data[0], src->data[0], src->linesize[0]);
    return 0;
}

void AlFFUtils::attachJvm(void *vm) {
    av_jni_set_java_vm(vm, NULL);
}

static void parseArgv(std::string cmd, std::vector<std::string> &vec) {
    size_t pos = cmd.find(" ");
    std::string arg;
    while (pos != string::npos) {
        if (pos > 0 && cmd[pos - 1] == '\\') {
            arg += cmd.substr(0, pos - 1);
            arg += " ";
        } else {
            arg += cmd.substr(0, pos);
            vec.push_back(arg);
            arg = "";
        }
        cmd = cmd.substr(pos + 1);
        pos = cmd.find(" ");
    }
    if (cmd.size() + arg.length()) {
        vec.push_back(arg + cmd);
    }
}

int AlFFUtils::exec(std::string cmd) {
    std::vector<std::string> vec;
    parseArgv(cmd, vec);

    int argc = static_cast<int>(vec.size());
    char **argv = new char *[argc + 1];
    for (int i = 0; i < argc; ++i) {
        argv[i] = new char[vec[i].size() + 1];;
        strcpy(argv[i], vec[i].c_str());
    }
    argv[argc] = nullptr;
    int ret = _exec(argc, argv);
    for (int i = 0; i < argc; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
    return ret;
}

int AlFFUtils::trackInfo(std::string &file) {
    init();
    int info = 0;
    auto *ctx = avformat_alloc_context();
    //打开输入视频文件
    if (avformat_open_input(&ctx, file.c_str(), nullptr, nullptr) != 0) {
        AlLogE(TAG, "failed. Couldn't open input stream.");
        avformat_free_context(ctx);
        return info;
    }

    auto track = av_find_best_stream(ctx, AVMediaType::AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (track >= 0) {
        info |= 0x1;
    }
    track = av_find_best_stream(ctx, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (track >= 0) {
        info |= 0x2;
    }
    avformat_close_input(&ctx);
    avformat_free_context(ctx);
    return info;
}

static void showEncoderInfo() {
    std::string msg;
    AVInputFormat *aif = av_iformat_next(nullptr);
    msg.append("ENCODERS: ");
    while (aif != nullptr) {
        msg.append(aif->name);
        msg.append(", ");
        aif = aif->next;
    }
    AlLogI(TAG, "%s", msg.c_str());
}

static void showDecoderInfo() {
    std::string msg;
    msg.append("DECODERS: ");
    AVOutputFormat *aof = av_oformat_next(nullptr);
    while (aof != nullptr) {
        msg.append(aof->name);
        msg.append(", ");
        aof = aof->next;
    }
    AlLogI(TAG, "%s", msg.c_str());
}

void AlFFUtils::showInfo() {
    init();
    AlLogI(TAG, "FFmpeg ver %d.%d.%d",
           LIBSWRESAMPLE_VERSION_MAJOR,
           LIBSWRESAMPLE_VERSION_MINOR,
           LIBSWRESAMPLE_VERSION_MICRO);
    showEncoderInfo();
    showDecoderInfo();
    std::string msg;
    msg.append("CODEC: ");
    AVCodec *codec = nullptr;
    while ((codec = av_codec_next(codec))) {
        msg.append(codec->name);
        msg.append(", ");
    }
    AlLogI(TAG, "%s", msg.c_str());
}

static bool _openTrack(AVFormatContext *ctx, int track, AVCodecContext **context) {
    AVCodecParameters *avCodecParameters = ctx->streams[track]->codecpar;
    AVCodec *codec = NULL;
    if (AV_CODEC_ID_H264 == avCodecParameters->codec_id) {
        codec = avcodec_find_decoder_by_name("h264_mediacodec");
        if (NULL == codec) {
            AlLogI(TAG, "Selected AV_CODEC_ID_H264.");
            codec = avcodec_find_decoder(avCodecParameters->codec_id);
        }
    } else {
        codec = avcodec_find_decoder(avCodecParameters->codec_id);
    }
    if (NULL == codec) {
        AlLogE(TAG, "Couldn't find codec.");
        return false;
    }
    //打开编码器
    *context = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(*context, avCodecParameters);
    if (avcodec_open2(*context, codec, NULL) < 0) {
        AlLogE(TAG, "Couldn't open codec.");
        return false;
    }
    string typeName = "unknown";
    if (AVMEDIA_TYPE_VIDEO == codec->type) {
        typeName = "video";
    } else if (AVMEDIA_TYPE_AUDIO == codec->type) {
        typeName = "audio";
    }
    AlLogI(TAG, "Open %s track with %s, fmt=%d, frameSize=%d", typeName.c_str(), codec->name,
           avCodecParameters->format, avCodecParameters->frame_size);
    return true;
}

static float _getAudioLevel(AVFrame *frame) {
    float result = 0.f;
    if (frame) {
        auto fmt = frame->format;
        auto *left = AlBuffer::wrap(frame->data[0], frame->linesize[0]);
        AlBuffer *right = nullptr;
        if (frame->linesize[1] > 0) {
            right = AlBuffer::wrap(frame->data[1], frame->linesize[1]);
        }
        switch (fmt) {
            case AV_SAMPLE_FMT_S16: {
                return left->getShort() * 1.f / 0x7FFF;
            }
            case AV_SAMPLE_FMT_FLTP: {
                result = left->getFloat();
                break;
            }
        }
        delete right;
        delete left;
    }
    return result;
}

int AlFFUtils::parseWaveform(int64_t seqIn, int64_t duInUS,
                             std::vector<std::string> &files,
                             std::vector<int64_t> &seqIns,
                             std::vector<int64_t> &trimIns,
                             std::vector<int64_t> &dus,
                             float *data,
                             int size) {
    float delta = duInUS / (float) size;
    int64_t current = -seqIn;
    int track = -1;
    int clipIndex = 0;
    int formIndex = 0;
    AVFormatContext *ctx = nullptr;
    AVCodecContext *c = nullptr;
    AVPacket *pkt = nullptr;
    AVFrame *frame = nullptr;
    while (current <= duInUS) {
        if (clipIndex < files.size() && current >= seqIns[clipIndex]) {
            if (c) {
                avcodec_close(c);
                c = nullptr;
            }
            if (ctx) {
                avformat_close_input(&ctx);
                avformat_free_context(ctx);
                ctx = nullptr;
            }
            if (pkt) {
                av_packet_free(&pkt);
                pkt = nullptr;
            }
            if (frame) {
                av_frame_free(&frame);
                frame = nullptr;
            }
            std::string path = files[clipIndex];
            int64_t trimIn = trimIns[clipIndex];
            ++clipIndex;
            ctx = avformat_alloc_context();
            if (avformat_open_input(&ctx, path.c_str(), NULL, NULL) != 0) {
                AlLogE(TAG, "Couldn't open input stream. index=%d/%d, cur=% " PRId64 ", %s",
                       clipIndex - 1, files.size(), current, path.c_str());
                return false;
            }
            track = av_find_best_stream(ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
            //获取视频文件信息
            if (avformat_find_stream_info(ctx, NULL) < 0) {
                AlLogE(TAG, "Couldn't find stream information.");
                return false;
            }
            if (AVERROR_STREAM_NOT_FOUND != track && !_openTrack(ctx, track, &c)) {
                AlLogE(TAG, "******** Open audio track failed. *********");
                return false;
            }
            pkt = av_packet_alloc();
            frame = av_frame_alloc();
        }
        int ret = avformat_seek_file(ctx, -1, INT64_MIN,
                                     current - seqIns[clipIndex - 1] + trimIns[clipIndex - 1],
                                     INT64_MAX, AVSEEK_FLAG_ANY);
        bool found = false;
        while (true) {
            int ret = av_read_frame(ctx, pkt);
//            AlLogI("alimin", "level %f", pkt->pts);
            if (0 == ret && track == pkt->stream_index) {
                avcodec_send_packet(c, pkt);
            }
            av_packet_unref(pkt);
            if (AVERROR_EOF == ret) {
                break;
            }
            if (0 == avcodec_receive_frame(c, frame)) {
                found = true;
                data[formIndex] = _getAudioLevel(frame);
                av_frame_unref(frame);
                break;
            }
        }
        if (!found) {
            data[formIndex] = 0;
        }
//        AlLogI("AlFFUtils", "%d, _getAudioLevel=%f", formIndex, data[formIndex]);
        ++formIndex;
        current += delta;
    }
    if (c) {
        avcodec_close(c);
        c = nullptr;
    }
    if (ctx) {
        avformat_close_input(&ctx);
        avformat_free_context(ctx);
        ctx = nullptr;
    }
    if (pkt) {
        av_packet_free(&pkt);
        pkt = nullptr;
    }
    if (frame) {
        av_frame_free(&frame);
        frame = nullptr;
    }
    return 0;
}

static void _addSequenceFrame(std::shared_ptr<AlMediaCoverSequence> sequence, SwsContext *sws, AVFrame *frame) {
    auto cover = std::make_shared<AlMediaCover>(64, 64);
    cover->setTimestamp(1);
    auto *dst = cover->buf()->data();
    int dstLineSize = 64 * 4;
    int ret = sws_scale(sws, frame->data, frame->linesize, 0, 64, &dst, &dstLineSize);
    if (ret <= 0) {
        AlLogE(TAG, "failed.");
    }
}

std::shared_ptr<AlMediaCoverSequence> AlFFUtils::parseVideoCover(int64_t seqIn, int64_t duInUS,
                                                                 std::vector<std::string> &files,
                                                                 std::vector<int64_t> &seqIns,
                                                                 std::vector<int64_t> &trimIns,
                                                                 std::vector<int64_t> &dus,
                                                                 int width) {
    int64_t current = -seqIn;
    int track = -1;
    int clipIndex = 0;
    AVFormatContext *ctx = nullptr;
    AVCodecContext *c = nullptr;
    SwsContext *sws = nullptr;
    AVPacket *pkt = nullptr;
    AVFrame *frame = nullptr;
    int64_t lastFramePts = -1;
    std::shared_ptr<AlMediaCoverSequence> sequence;
    while (current <= duInUS) {
        if (clipIndex < files.size() && current >= seqIns[clipIndex]) {
            if (sws) {
                sws_freeContext(sws);
                sws = nullptr;
            }
            if (c) {
                avcodec_close(c);
                c = nullptr;
            }
            if (ctx) {
                avformat_close_input(&ctx);
                avformat_free_context(ctx);
                ctx = nullptr;
            }
            if (pkt) {
                av_packet_free(&pkt);
                pkt = nullptr;
            }
            if (frame) {
                av_frame_free(&frame);
                frame = nullptr;
            }
            std::string path = files[clipIndex];
            int64_t trimIn = trimIns[clipIndex];
            ++clipIndex;
            ctx = avformat_alloc_context();
            if (avformat_open_input(&ctx, path.c_str(), NULL, NULL) != 0) {
                AlLogE(TAG, "Couldn't open input stream. index=%d/%d, cur=% " PRId64 ", %s",
                       clipIndex - 1, files.size(), current, path.c_str());
                return nullptr;
            }
            track = av_find_best_stream(ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
            //获取视频文件信息
            if (avformat_find_stream_info(ctx, NULL) < 0) {
                AlLogE(TAG, "Couldn't find stream information.");
                return nullptr;
            }
            if (AVERROR_STREAM_NOT_FOUND != track && !_openTrack(ctx, track, &c)) {
                AlLogE(TAG, "******** Open video track failed. *********");
                return nullptr;
            }
            pkt = av_packet_alloc();
            frame = av_frame_alloc();
            auto srcW = ctx->streams[track]->codecpar->width;
            auto srcH = ctx->streams[track]->codecpar->height;
            auto srcFmt = (AVPixelFormat) ctx->streams[track]->codecpar->format;
            auto dstW = 64;
            auto dstH = 64;
            sws = sws_getContext(srcW, srcH, srcFmt,
                                 dstW, dstH, AVPixelFormat::AV_PIX_FMT_RGBA,
                                 SWS_BICUBIC, nullptr, nullptr, nullptr);
        }
        int64_t time = current - seqIns[clipIndex - 1] + trimIns[clipIndex - 1];
        int64_t trackTime = av_index_search_timestamp(ctx->streams[track], time,
                                                    AVSEEK_FLAG_BACKWARD);
        if (lastFramePts != trackTime) {
            lastFramePts = trackTime;
            int ret = avformat_seek_file(ctx, -1, INT64_MIN, time,
                                         INT64_MAX, AVSEEK_FLAG_BACKWARD);
            while (true) {
                int ret = av_read_frame(ctx, pkt);
                if (0 == ret && track == pkt->stream_index) {
                    avcodec_send_packet(c, pkt);
                }
                av_packet_unref(pkt);
                if (AVERROR_EOF == ret) {
                    break;
                }
                if (0 == avcodec_receive_frame(c, frame)) {
                    _addSequenceFrame(sequence, sws, frame);
                    av_frame_unref(frame);
                    break;
                }
            }
        }
        current += 33000;
    }
    if (sws) {
        sws_freeContext(sws);
        sws = nullptr;
    }
    if (c) {
        avcodec_close(c);
        c = nullptr;
    }
    if (ctx) {
        avformat_close_input(&ctx);
        avformat_free_context(ctx);
        ctx = nullptr;
    }
    if (pkt) {
        av_packet_free(&pkt);
        pkt = nullptr;
    }
    if (frame) {
        av_frame_free(&frame);
        frame = nullptr;
    }
    return nullptr;
}
