//
// Created by mingyi.li on 4/8/22.
//

#include "FkJavaFunc.h"

FkJavaFunc::FkJavaFunc(JNIEnv *env, jobject obj, std::string name, std::string sig) {
    auto cls = env->GetObjectClass(obj);
    mid = env->GetMethodID(cls, name.c_str(), sig.c_str());
}

FkJavaFunc::~FkJavaFunc() {
    mid = 0;
}

bool FkJavaFunc::call(JNIEnv *env, jobject obj, ...) {
    if (mid == 0) {
        return false;
    }
    va_list args;
    va_start(args, obj);
    auto ret = env->CallBooleanMethod(obj, mid, args);
    va_end(args);
    return ret;
}
