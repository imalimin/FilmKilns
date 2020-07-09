/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
#ifndef HARDWAREVIDEOCODEC_OBJECT_H
#define HARDWAREVIDEOCODEC_OBJECT_H

#include <cstring>
#include <cstdlib>
#include <string>

using namespace std;

#define AL_CLASS class
#define AL_INTERFACE class
#define AL_EXTEND : public
#define AL_NULLABLE
#define AL_ENUM enum class
#define AL_INSTANCE_OF(o1, o2) (nullptr != dynamic_cast<o2>(o1))

#define AL_CHECK(flag, ret) \
if(!(flag)) { \
    return (ret); \
} \

#define al_class AL_CLASS
#define al_interface AL_INTERFACE
#define al_extend AL_EXTEND

template<class T>
class AlSPointer;

class Object {
public:
    Object();

    virtual ~Object();

    virtual bool equals(const Object &o);

    virtual std::string toString();

};

#endif //HARDWAREVIDEOCODEC_OBJECT_H
