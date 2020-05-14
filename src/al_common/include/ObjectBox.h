//
// Created by limin on 2018/12/26.
//

#ifndef HARDWAREVIDEOCODEC_OBJECTBOX_H
#define HARDWAREVIDEOCODEC_OBJECTBOX_H

#include "Object.h"

al_class ObjectBox al_extend Object {
public:
    static ObjectBox *box(void *ptr);

    static ObjectBox *wrap(Object *obj);

    void *ptr = nullptr;
    Object *obj = nullptr;

    ObjectBox(void *ptr);

    virtual ~ObjectBox();

    template<class T>
    T unWrap() {
        return dynamic_cast<T>(obj);
    }
};


#endif //HARDWAREVIDEOCODEC_OBJECTBOX_H
