/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlTextWriter.h"
#include "StringUtils.h"

AlTextWriter::AlTextWriter(std::string path, bool appendMode) : AlAbsFileWriter(path) {
    if (appendMode) {
        file = fopen(path.c_str(), "a+");
    } else {
        file = fopen(path.c_str(), "w");
    }
}

AlTextWriter::~AlTextWriter() {
}

void AlTextWriter::close() {
    if (file) {
        fclose(file);
        file = nullptr;
    }
}

void AlTextWriter::append(char c) {
    if (nullptr == file)
        return;
    fputc(c, file);
}

void AlTextWriter::append(std::string text) {
    if (nullptr == file)
        return;
    fputs(text.c_str(), file);
}

void AlTextWriter::append(int32_t value) {
    if (nullptr == file)
        return;
    append(StringUtils::valueOf(value));
}

void AlTextWriter::append(int64_t value) {
    if (nullptr == file)
        return;
    append(StringUtils::valueOf(value));
}

void AlTextWriter::append(float value) {
    if (nullptr == file)
        return;
    append(StringUtils::valueOf(value));
}

void AlTextWriter::append(double value) {
    if (nullptr == file)
        return;
    append(StringUtils::valueOf(value));
}
