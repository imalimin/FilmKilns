/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALPARCEL_H
#define HWVC_ANDROID_ALPARCEL_H

#include "Object.h"
#include "AlBuffer.h"
#include <vector>

class AlParcelable;

AL_CLASS AlParcel AL_EXTEND Object {
public:
    AlParcel();

    virtual ~AlParcel();

    void writeByte(uint8_t val);

    void writeInt(int32_t val);

    void writeLong(int64_t val);

    void writeFloat(float val);

    void writeDouble(double val);

    void writeString(std::string &val);

    void writeByteArray(std::vector<uint8_t> &vec);

    void writeByteArray(uint8_t *data, size_t size);

    void writeIntArray(std::vector<int32_t> &vec);

    void writeLongArray(std::vector<int64_t> &vec);

    void writeFloatArray(std::vector<float> &vec);

    void writeDoubleArray(std::vector<double> &vec);

    void writeStringArray(std::vector<std::string> &vec);

    void writeParcelArray(std::vector<AlParcelable *> &vec);

private:
    AlParcel(const AlParcel &o) : Object() {};

    friend AlParcelable;

    std::shared_ptr<AlBuffer> data();

    void writeBegin();

    void writeEnd();

private:
    std::vector<uint8_t> vec;
};

AL_CLASS AlParcelable AL_EXTEND Object {
public:
    AlParcelable();

    AlParcelable(const AlParcelable &o);

    virtual ~AlParcelable();

    std::shared_ptr<AlBuffer> data();

    virtual void writeToParcel(std::shared_ptr<AlParcel> parcel) = 0;

private:
    std::shared_ptr<AlParcel> parcel = nullptr;
};


#endif //HWVC_ANDROID_ALPARCEL_H
