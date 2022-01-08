/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "FkTestResultHolder.h"

FkTestResultHolder *FkTestResultHolder::instance = new FkTestResultHolder();

FkTestResultHolder::FkTestResultHolder() {

}

FkTestResultHolder *FkTestResultHolder::getInstance() {
    return instance;
}

void FkTestResultHolder::push(std::string msg, bool _passed) {
    if (_passed) {
        passed.push_back(msg);
    } else {
        failed.push_back(msg);
    }
}

void FkTestResultHolder::clear() {
    passed.clear();
    failed.clear();
}

void FkTestResultHolder::pop(std::vector<std::string> &vec) {
//    for (auto &it : passed) {
//        vec.push_back(it);
//    }
    for (auto &it : failed) {
        vec.push_back(it);
    }
}
