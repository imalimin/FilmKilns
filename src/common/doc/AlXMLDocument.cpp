//
// Created by mingyi.li on 2019-12-30.
//

#include "AlXMLDocument.h"
#include "AlElement.h"
#include "StringUtils.h"
#include "Logcat.h"
#include "tinyxml/tinyxml.h"

#define TAG "AlXMLDocument"


static AlElement *_read(TiXmlElement *elm) {
    string name = elm->Value();
    if (StringUtils::isEmpty(&name)) {
        Logcat::e(TAG, "%s(%d) _read failed.", __FUNCTION__, __LINE__);
        return nullptr;
    }
    AlElement *e = new AlElement(name);
    const char *text = elm->GetText();
    if (text) {
        e->setText(text);
    }
    for (TiXmlAttribute *attr = elm->FirstAttribute(); NULL != attr; attr = attr->Next()) {
        e->addAttr(attr->Name(), attr->Value());
    }
    for (TiXmlElement *ee = elm->FirstChildElement(); NULL != ee; ee = ee->NextSiblingElement()) {
        AlElement *child = _read(ee);
        if (child) {
            e->addChild(child);
        }
    }
    return e;
}

AlXMLDocument *AlXMLDocument::fromFile(std::string file) {
    TiXmlDocument doc;
    if (!doc.LoadFile(file.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8)) {
        doc.Clear();
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return nullptr;
    }

    TiXmlElement *tiRoot = doc.FirstChildElement();
    if (nullptr == tiRoot) {
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        doc.Clear();
        return nullptr;
    }
    AlElement *root = _read(tiRoot);
    doc.Clear();
    if (nullptr == root) {
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return nullptr;
    }
    return new AlXMLDocument(root);
}

AlXMLDocument::AlXMLDocument(AlElement *root) : Object(), _root(root) {

}

AlXMLDocument::~AlXMLDocument() {
    delete _root;
    _root = nullptr;
}

AlElement *AlXMLDocument::root() {
    return _root;
}
