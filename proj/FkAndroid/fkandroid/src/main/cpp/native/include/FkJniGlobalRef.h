//
// Created by Alimin on 2022/4/10.
//

#ifndef FKANDROID_FKJNIGLOBALREF_H
#define FKANDROID_FKJNIGLOBALREF_H

#include "FkObject.h"
#include <jni.h>

FK_CLASS FkJniGlobalRef FK_EXTEND FkObject {
public:
    FkJniGlobalRef(jobject obj);

    FkJniGlobalRef(const FkJniGlobalRef &o) = delete;

    virtual ~FkJniGlobalRef();

    jobject obj();

private:
    jobject _obj = nullptr;
};


#endif //FKANDROID_FKJNIGLOBALREF_H
