/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALTEXTWRITER_H
#define HWVC_ANDROID_ALTEXTWRITER_H

#include "AlAbsFileWriter.h"

al_class_ex(AlTextWriter, AlAbsFileWriter) {
public:
    AlTextWriter(std::string path, bool appendMode = false);

    virtual ~AlTextWriter();

    virtual void close() override;

    void append(char c);

    void append(std::string text);

    void append(int32_t value);

    void append(int64_t value);

    void append(float value);

    void append(double value);

private:
    FILE *file = nullptr;
};


#endif //HWVC_ANDROID_ALTEXTWRITER_H
