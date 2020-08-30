/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "platform/android/AlJavaRuntime.h"
#include "platform/android/AlJNIObject.h"
#include "AlFFUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_tools_AlFFUtils_exec(JNIEnv *env,
                                                                    jobject thiz,
                                                                    jstring cmd) {
    const char *pCmd = env->GetStringUTFChars(cmd, JNI_FALSE);
    std::string str = std::string(pCmd);
    env->ReleaseStringChars(cmd, reinterpret_cast<const jchar *>(pCmd));
    return AlFFUtils::exec(str);
}

JNIEXPORT jint JNICALL Java_com_lmy_hwvcnative_tools_AlFFUtils_trackInfo(JNIEnv *env,
                                                                         jobject thiz,
                                                                         jstring file) {
    const char *pFile = env->GetStringUTFChars(file, JNI_FALSE);
    std::string str = std::string(pFile);
    env->ReleaseStringChars(file, reinterpret_cast<const jchar *>(pFile));
    return AlFFUtils::trackInfo(str);
}

JNIEXPORT jfloatArray JNICALL Java_com_lmy_hwvcnative_tools_AlFFUtils_parseWaveform(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jlong seqIn,
                                                                                   jlong duInUS,
                                                                                   jobjectArray files,
                                                                                   jlongArray seqIns,
                                                                                   jlongArray trimIns,
                                                                                   jlongArray dus,
                                                                                   jint width) {
    if (nullptr == files || nullptr == seqIns || nullptr == trimIns || nullptr == dus ||
        width <= 0) {
        AlLogE("Java_AlFFUtils", "Invalid params");
        return nullptr;
    }
    auto count = env->GetArrayLength(files);
    if (0 == count || count != env->GetArrayLength(seqIns) ||
        count != env->GetArrayLength(trimIns) ||
        count != env->GetArrayLength(dus)) {
        AlLogE("Java_AlFFUtils", "Invalid params");
        return nullptr;
    }
    std::vector<std::string> vecOfFiles;
    std::vector<int64_t> vecOfSeqIns;
    std::vector<int64_t> vecOfTrimIns;
    std::vector<int64_t> vecOfDus;
    auto pSeqIns = env->GetLongArrayElements(seqIns, nullptr);
    auto pTrimIns = env->GetLongArrayElements(trimIns, nullptr);
    auto pDus = env->GetLongArrayElements(dus, nullptr);
    for (int i = 0; i < count; ++i) {
        jstring oStr = (jstring) env->GetObjectArrayElement(files, i);
        const char *pFile = env->GetStringUTFChars(oStr, JNI_FALSE);
        vecOfFiles.push_back(std::string(pFile));
        env->ReleaseStringChars(oStr, reinterpret_cast<const jchar *>(pFile));
        env->DeleteLocalRef(oStr);
        vecOfSeqIns.push_back(pSeqIns[0]);
        vecOfTrimIns.push_back(pTrimIns[0]);
        vecOfDus.push_back(pDus[0]);
    }
    env->ReleaseLongArrayElements(dus, pDus, 0);
    env->ReleaseLongArrayElements(trimIns, pTrimIns, 0);
    env->ReleaseLongArrayElements(seqIns, pSeqIns, 0);

    auto floatArray = env->NewFloatArray(width);
    auto *data = env->GetFloatArrayElements(floatArray, NULL);
    memset(data, 0, width * sizeof(float));
    int ret = AlFFUtils::parseWaveform(seqIn, duInUS, vecOfFiles, vecOfSeqIns,
                                       vecOfTrimIns, vecOfDus, data, width);
    env->ReleaseFloatArrayElements(floatArray, data, 0);
    return floatArray;
}

#ifdef __cplusplus
}
#endif