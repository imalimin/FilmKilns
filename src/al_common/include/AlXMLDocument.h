//
// Created by mingyi.li on 2019-12-30.
//

#ifndef HWVC_ANDROID_ALXMLDOCUMENT_H
#define HWVC_ANDROID_ALXMLDOCUMENT_H

#include "Object.h"
#include "AlElement.h"

al_class(AlXMLDocument) {
public:
    static AlXMLDocument *fromFile(std::string file);

    ~AlXMLDocument();

    AlElement *root();

private:
    AlXMLDocument(AlElement *root);

    AlXMLDocument(const AlXMLDocument &o) : Object() {};

private:
    AlElement *_root = nullptr;
};


#endif //HWVC_ANDROID_ALXMLDOCUMENT_H
