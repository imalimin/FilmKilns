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

int FFUtils::exec(std::string cmd) {
    int pos = cmd.find(" ");
    std::string subStr = "";
    vector<string> listArgv;
    while (pos != string::npos) {
        if (pos > 0 && cmd[pos - 1] == '\\') {
            subStr += cmd.substr(0, pos - 1);
            subStr += " ";
        } else {
            subStr += cmd.substr(0, pos);
            listArgv.push_back(subStr);
            subStr = "";
        }
        cmd = cmd.substr(pos + 1);
        pos = cmd.find(" ");
    }
    if (cmd.size() + subStr.length()) {
        listArgv.push_back(subStr + cmd);
    }

    int argc = listArgv.size();
    char **argv = (char **) malloc(sizeof(char *) * (argc + 1));
    for (int i = 0; i < argc; ++i) {
        argv[i] = (char *) malloc(sizeof(char) * listArgv[i].size() + 1);
        strcpy(argv[i], listArgv[i].c_str());
    }
    argv[argc] = nullptr;
    int ret = _exec(argc, argv);

    for (int i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    free(argv);

    return ret;
}
