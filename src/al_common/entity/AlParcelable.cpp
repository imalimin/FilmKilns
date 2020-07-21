//
// Created by limin on 2020/7/21.
//

#include "AlParcelable.h"

AlParcel::AlParcel() : Object() {

}

AlParcel::~AlParcel() {

}

std::shared_ptr<AlBuffer> AlParcel::data() {
    AlBuffer *buf = AlBuffer::alloc(vec.size());
    buf->put(vec.data(), vec.size());
    buf->rewind();
    return std::shared_ptr<AlBuffer>(buf);
}

void AlParcel::writeBegin() {

}

void AlParcel::writeEnd() {

}

void AlParcel::writeByte(uint8_t val) {

}

void AlParcel::writeInt(int32_t val) {

}

void AlParcel::writeLong(int64_t val) {

}

void AlParcel::writeFloat(float val) {

}

void AlParcel::writeDouble(double val) {

}

void AlParcel::writeString(std::string &val) {

}

void AlParcel::writeByteArray(std::vector<uint8_t> &vec) {

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