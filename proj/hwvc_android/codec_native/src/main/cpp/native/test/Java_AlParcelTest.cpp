/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "platform/android/AlJNIEnv.h"
#include "platform/android/AlJNIObject.h"
#include "AlParcelable.h"

AL_CLASS AlTestModel AL_EXTEND AlParcelable {
public:
    AlTestModel(int32_t int0, float flt0, int32_t int1, double d0)
            : AlParcelable(), int0(int0), flt0(flt0), int1(int1), d0(d0) {

    };

    virtual ~AlTestModel() {

    };

    virtual void writeToParcel(std::shared_ptr<AlParcel> parcel) override {
        parcel->writeInt(int0);
        parcel->writeFloat(flt0);
        parcel->writeInt(int1);
        parcel->writeDouble(d0);
    };

private:
    int32_t int0;
    float flt0;
    int32_t int1;
    double d0;
};

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT jbyteArray JNICALL Java_com_lmy_hwvcnative_test_AlParcelTest_testParcel
        (JNIEnv *env, jobject thiz) {
    AlTestModel model(123456, 3.14159f, 234567, 3.141592653);
    auto buf = model.data();
    buf->rewind();
    auto data = env->NewByteArray(buf->size());
    env->SetByteArrayRegion(data, 0, buf->size(), reinterpret_cast<const jbyte *>(buf->data()));
    return data;
}

#ifdef __cplusplus
}
#endif