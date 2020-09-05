/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlParcelable.h"

#define AL_CHECK_PRI_SIZE(pri, size) \
if (size != sizeof(pri)) { \
    return; \
} \

AlParcel::AlParcel() : Object() {

}

AlParcel::~AlParcel() {

}

std::shared_ptr<AlBuffer> AlParcel::data() {
    if (vec.empty()) {
        return nullptr;
    }
    AlBuffer *buf = AlBuffer::alloc(vec.size());
    buf->put(vec.data(), vec.size());
    buf->rewind();
    return std::shared_ptr<AlBuffer>(buf);
}

void AlParcel::writeBegin() {
    vec.clear();
}

void AlParcel::writeEnd() {

}

void AlParcel::writeByte(uint8_t val) {
    AL_CHECK_PRI_SIZE(uint8_t, 1)
    vec.emplace_back(val);
}

void AlParcel::writeInt(int32_t val) {
    AL_CHECK_PRI_SIZE(int32_t, 4)
    for (int i = 0; i < 4; ++i) {
        vec.emplace_back((val >> (i * 8)) & 0xFF);
    }
}

void AlParcel::writeLong(int64_t val) {
    AL_CHECK_PRI_SIZE(int64_t, 8)
    for (int i = 0; i < 8; ++i) {
        vec.emplace_back((val >> (i * 8)) & 0xFF);
    }
}

void AlParcel::writeFloat(float val) {
    AL_CHECK_PRI_SIZE(float, 4)
    auto *src = reinterpret_cast<uint8_t *>(&val);
    for (int i = 0; i < 4; ++i) {
        vec.emplace_back(src[i]);
    }
}

void AlParcel::writeDouble(double val) {
    AL_CHECK_PRI_SIZE(double, 8)
    auto *src = reinterpret_cast<uint8_t *>(&val);
    for (int i = 0; i < 8; ++i) {
        vec.emplace_back(src[i]);
    }
}

void AlParcel::writeString(std::string &val) {
    size_t len = val.length();
    const char *str = val.c_str();
    writeInt(len);
    for (int i = 0; i < len; ++i) {
        vec.emplace_back(str[i]);
    }
}

void AlParcel::writeByteArray(std::vector<uint8_t> &vec) {
    writeByteArray(vec.data(), vec.size());
}

void AlParcel::writeByteArray(uint8_t *data, size_t size) {
    writeInt(size);
    for (int i = 0; i < size; ++i) {
        writeByte(data[i]);
    }
}

void AlParcel::writeIntArray(std::vector<int32_t> &vec) {

}

void AlParcel::writeLongArray(std::vector<int64_t> &vec) {

}

void AlParcel::writeFloatArray(std::vector<float> &vec) {

}

void AlParcel::writeDoubleArray(std::vector<double> &vec) {

}

void AlParcel::writeStringArray(std::vector<std::string> &vec) {

}

void AlParcel::writeParcelArray(std::vector<AlParcelable *> &vec) {
    auto size = vec.size();
    writeInt(vec.size());
    std::shared_ptr<AlParcel> parcel(this, [](AlParcel *p) {});
    for (int i = 0; i < size; ++i) {
        vec[i]->writeToParcel(parcel);
    }
}

AlParcelable::AlParcelable() : Object() {
    parcel = std::make_shared<AlParcel>();
}

AlParcelable::AlParcelable(const AlParcelable &o) : Object() {
    parcel = o.parcel;
}

AlParcelable::~AlParcelable() {

}

std::shared_ptr<AlBuffer> AlParcelable::data() {
    parcel->writeBegin();
    writeToParcel(parcel);
    parcel->writeEnd();
    return parcel->data();
}