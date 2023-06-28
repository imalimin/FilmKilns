//
// Created by mingyi.li on 4/8/22.
//

#ifndef FKANDROID_FKJAVAFUNC_H
#define FKANDROID_FKJAVAFUNC_H

#include "jni.h"
#include <string>

class FkJavaFunc {
public:
    static std::shared_ptr<FkJavaFunc> makeNativeMsgListener(JNIEnv *env, jobject obj);

public:
    FkJavaFunc(JNIEnv *env, jobject obj, std::string name, std::string sig);

    FkJavaFunc(const FkJavaFunc &o) = delete;

    virtual ~FkJavaFunc();

    bool call(JNIEnv *env, jobject obj, ...);

private:
    jmethodID mid = NULL;
};


#endif //FKANDROID_FKJAVAFUNC_H
