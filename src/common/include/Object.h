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

#define al_class_ex(cls, parent) \
class cls : public parent \

#define al_class(cls) al_class_ex(cls, Object)

#define al_interface(cls) \
class cls : public Object \


template<class T>
class AlSPointer;

class Object {
public:
    Object();

    virtual ~Object();

    virtual std::string toString();

private:
    template<class T>
    friend
    class AlSPointer;

    void ref();

    void unref();

private:
    int32_t countOfRef = 0;
};

template<class T>
al_class(AlSPointer) {
public:
    AlSPointer(Object *obj);

    AlSPointer(const AlSPointer &o);

    ~AlSPointer();

    AlSPointer<T> &operator=(AlSPointer<T> &o);

    T *operator->();

    template<class D>
    D *as();

private:
    Object *obj = nullptr;
};

template<class T>
AlSPointer<T>::AlSPointer(Object *obj) : Object(), obj(nullptr) {
    if (obj) {
        obj->ref();
        this->obj = obj;
    }
}

template<class T>
AlSPointer<T>::AlSPointer(const AlSPointer &o) : Object() {
    if (o.obj) {
        obj->ref();
        this->obj = o.obj;
    }
}

template<class T>
AlSPointer<T>::~AlSPointer() {
    if (obj) {
        obj->unref();
        if (obj->countOfRef <= 0) {
            delete obj;
            this->obj = nullptr;
        }
    }
}

template<class T>
AlSPointer<T> &AlSPointer<T>::operator=(AlSPointer<T> &o) {
    static AlSPointer<T> *p;
    if (p == nullptr) {
        p = new AlSPointer<T>(o.obj);
    }
    return *p;
}

template<class T>
T *AlSPointer<T>::operator->() {
    return obj;
}

template<class T>
template<class D>
D *AlSPointer<T>::as() {
    if (nullptr != obj) {
        return dynamic_cast<D *>(obj);
    }
    return nullptr;
}

#endif //HARDWAREVIDEOCODEC_OBJECT_H
