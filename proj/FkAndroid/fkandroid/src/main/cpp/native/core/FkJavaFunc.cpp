//
// Created by mingyi.li on 4/8/22.
//

#include "FkJavaFunc.h"

FkJavaFunc::FkJavaFunc(JNIEnv *env, jobject obj, std::string name, std::string sig) {
    JavaVM *vm = nullptr;
    auto ret = env->GetJavaVM(&vm);
    auto cls = env->GetObjectClass(obj);
    mid = env->GetMethodID(cls, name.c_str(), sig.c_str());
}

FkJavaFunc::~FkJavaFunc() {
    mid = NULL;
}

bool FkJavaFunc::call(JNIEnv *env, jobject obj, ...) {
    if (mid == NULL) {
        return false;
    }
    va_list args;
    va_start(args, obj);
    auto ret = env->CallBooleanMethod(obj, mid, args);
    va_end(args);
    return ret;
}

std::shared_ptr<FkJavaFunc> FkJavaFunc::makeNativeMsgListener(JNIEnv *env, jobject obj) {
    return std::make_shared<FkJavaFunc>(env, obj, "onNativeMsgReceived",
                                        "(IILjava/lang/String;Ljava/nio/ByteBuffer;)Z");
}
