/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FFUtils.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
#include "fftools/ffmpeg.h"
#ifdef __cplusplus
}
#endif

int FFUtils::avSamplesCopy(AVFrame *dest, AVFrame *src) {
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

void FFUtils::attachJvm(void *vm) {
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

int FFUtils::exec(std::string cmd) {
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
