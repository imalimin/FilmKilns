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

}