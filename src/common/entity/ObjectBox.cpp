//
// Created by limin on 2018/12/26.
//

#include "../include/ObjectBox.h"

ObjectBox *ObjectBox::box(void *ptr) {
    return new ObjectBox(ptr);
}

ObjectBox::ObjectBox(void *ptr) : Object() {
    this->ptr = ptr;
}

ObjectBox::~ObjectBox() {
    this->ptr = nullptr;
}
