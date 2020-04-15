/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlElement.h"
#include "StringUtils.h"
#include "Logcat.h"

#define TAG "AlElement"

AlElement::AlElement(std::string name) : Object(), _name(name) {

}

AlElement::AlElement(std::string name, std::vector<AlAttribute> &attrs) : Object(), _name(name) {
    size_t size = attrs.size();
    for (int i = 0; i < size; ++i) {
        this->attrs.insert(pair<std::string, AlAttribute>(attrs[i].name(), attrs[i]));
    }
}

AlElement::~AlElement() {
    size_t size = childes.size();
    for (int i = 0; i < size; ++i) {
        delete childes[i];
    }
    childes.clear();
    this->attrs.clear();
}

void AlElement::addAttr(std::string &name, std::string &value) {
    addAttr(name.c_str(), value);
}

void AlElement::addAttr(const char *name, std::string value) {
    if (this->attrs.end() != this->attrs.find(name)) {
        Logcat::w(TAG, "%s(%d) addAttr failed.", __FUNCTION__, __LINE__);
        return;;
    }
    std::string key(name);
    this->attrs.insert(pair<std::string, AlAttribute>(key, AlAttribute(key, value)));
}

size_t AlElement::size() {
    return childes.size();
}

void AlElement::addChild(AlElement *e) {
    if (contains(e)) {
        Logcat::w(TAG, "%s(%d) addChild failed.", __FUNCTION__, __LINE__);
        return;;
    }
    childes.push_back(e);
}

AlElement *AlElement::childAt(int32_t index) {
    if (index >= size()) return nullptr;
    return childes[index];
}

bool AlElement::contains(AlElement *e) {
    size_t size = childes.size();
    for (int i = 0; i < size; ++i) {
        if (childes[i]->contains(e)) {
            return true;
        }
    }
    return false;
}

std::string AlElement::toString() {
    std::string xml;
    xml.append("<");
    xml.append(_name);
    xml.append(" ");
    auto itr = attrs.begin();
    while (attrs.end() != itr) {
        AlAttribute attr = itr->second;
        xml.append(attr.name());
        xml.append("=\"");
        xml.append(attr.value());
        xml.append("\" ");
        ++itr;
    }
    xml.append(">");
    size_t len = childes.size();
    if (len > 0) {
        xml.append("\n");
    }
    for (int i = 0; i < len; ++i) {
//        xml.append("    ");
        xml.append(childes[i]->toString());
    }
    xml.append("</");
    xml.append(_name);
    xml.append(">\n");
    return xml;
}

std::string AlElement::name() {
    return _name;
}

std::string AlElement::text() {
    return _text;
}

void AlElement::setText(std::string text) {
    this->_text = text;
}

std::string AlElement::attr(const char *name) {
    auto itr = attrs.find(name);
    if (attrs.end() == itr) {
        return "";
    }
    return itr->second.value();
}

bool AlElement::nameIs(const char *name) {
    return StringUtils::equalsIgnoreCase(name, _name);
}

int32_t AlElement::attrInt(const char *name) {
    return StringUtils::toInt(attr(name).c_str());
}

float AlElement::attrFloat(const char *name) {
    return StringUtils::toFloat(attr(name).c_str());
}
