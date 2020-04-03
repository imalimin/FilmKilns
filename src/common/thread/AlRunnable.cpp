//
// Created by limin on 2019/12/19.
//

#include "../include/AlRunnable.h"

AlRunnable::AlRunnable(std::function<void(Object *obj)> runnable) : runnable(runnable) {

}

AlRunnable::~AlRunnable() {
    runnable = nullptr;
}

void AlRunnable::operator()(Object *obj) {
    if (runnable) {
        runnable(obj);
        runnable = nullptr;
    }
}
