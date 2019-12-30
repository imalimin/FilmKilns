/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALELEMENT_H
#define HWVC_ANDROID_ALELEMENT_H

#include "Object.h"
#include "AlAttribute.h"
#include <vector>
#include <map>

al_class(AlElement) {
public:
    AlElement(std::string name);

    AlElement(std::string name, std::vector<AlAttribute> &attrs);

    virtual ~AlElement();

    void addAttr(std::string &name, std::string &value);

    void addAttr(const char *name, std::string value);

    size_t size();

    void addChild(AlElement *e);

    AlElement *childAt(int32_t index);

    bool contains(AlElement *e);

    virtual std::string toString() override ;

private:
    AlElement(const AlElement &o) : Object() {};
private:
    std::string name;
    std::map<std::string, AlAttribute> attrs;
    std::vector<AlElement *> childs;
};


#endif //HWVC_ANDROID_ALELEMENT_H
