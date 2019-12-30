/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlElement.h"
#include "Logcat.h"

#define TAG "AlElement"

AlElement::AlElement(std::string name) : Object(), name(name) {

}

AlElement::AlElement(std::string name, std::vector<AlAttribute> &attrs) : Object(), name(name) {
    size_t size = attrs.size();
    for (int i = 0; i < size; ++i) {
        this->attrs.insert(pair<std::string, AlAttribute>(attrs[i].name(), attrs[i]));
    }
}

AlElement::~AlElement() {
    size_t size = childs.size();
    for (int i = 0; i < size; ++i) {
        delete childs[i];
    }
    childs.clear();
    this->attrs.clear();
}

void AlElement::addAttr(std::string &name, std::string &value) {
    this->attrs.insert(pair<std::string, AlAttribute>(name, AlAttribute(name, value)));
}

void AlElement::addAttr(const char *name, std::string value) {
    std::string key(name);
    this->attrs.insert(pair<std::string, AlAttribute>(key, AlAttribute(key, value)));
}

size_t AlElement::size() {
    return childs.size();
}

void AlElement::addChild(AlElement *e) {
    if (contains(e)) {
        Logcat::w(TAG, "%s(%d) addChild failed.", __FUNCTION__, __LINE__);
        return;;
    }
    childs.push_back(e);
}

AlElement *AlElement::childAt(int32_t index) {
    if (index >= size()) return nullptr;
    return childs[index];
}

bool AlElement::contains(AlElement *e) {
    size_t size = childs.size();
    for (int i = 0; i < size; ++i) {
        if (childs[i]->contains(e)) {
            return true;
        }
    }
    return false;
}
