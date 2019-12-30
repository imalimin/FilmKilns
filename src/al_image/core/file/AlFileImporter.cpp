/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFileImporter.h"
#include "AlXmlTagDefine.h"
#include "AlOperateFactory.h"
#include "AlCropOperateModel.h"
#include "StringUtils.h"
#include "AlObjectGuard.h"
#include "Logcat.h"

#define TAG "AlFileImporter"

AlFileImporter::AlFileImporter() : Object() {

}

AlFileImporter::~AlFileImporter() {

}

HwResult AlFileImporter::importFromFile(std::string inFile, AlImageCanvasModel *canvas,
                                        std::vector<AlImageLayerModel *> *layers) {
    TiXmlDocument doc;
    if (!doc.LoadFile(inFile.c_str(), TiXmlEncoding::TIXML_ENCODING_UTF8)) {
        doc.Clear();
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    AlElement *root = nullptr;
    _read(&doc, &root);
    doc.Clear();
    if (nullptr == root) {
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    AlObjectGuard guard((Object **) &root);
    if (!root->nameIs(TAG_ROOT)) {
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    size_t size = root->size();
    for (int i = 0; i < size; ++i) {
        AlElement *child = root->childAt(i);
        if (child->nameIs(TAG_CANVAS)) {
            if (Hw::SUCCESS != canvas->fromElement(child)) {
                Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
                return Hw::FAILED;
            }
        } else if (child->nameIs(TAG_LAYER)) {
            auto *layer = AlImageLayerModel::create(0, "");
            if (Hw::SUCCESS != layer->fromElement(child)) {
                Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
                delete layer;
                return Hw::FAILED;
            }
            layers->push_back(layer);
        }
    }
    return Hw::SUCCESS;
}

void AlFileImporter::_read(TiXmlDocument *doc, AlElement **element) {
    TiXmlElement *tiRoot = doc->FirstChildElement();
    if (nullptr == tiRoot) {
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return;
    }
    string rootName = tiRoot->Value();
    if (!StringUtils::equalsIgnoreCase(TAG_ROOT, rootName)) {
        Logcat::e(TAG, "%s(%d) invalid qua file.", __FUNCTION__, __LINE__);
        return;
    }
    AlElement *root = _parseElement(tiRoot);
    *element = root;
}

HwResult AlFileImporter::importFromStr(std::string *inStr, AlImageCanvasModel *canvas,
                                       std::vector<AlImageLayerModel *> *layers) {
    return Hw::SUCCESS;
}

AlElement *AlFileImporter::_parseElement(TiXmlElement *elm) {
    string name = elm->Value();
    if (StringUtils::isEmpty(&name)) {
        Logcat::e(TAG, "%s(%d) invalid qua file.", __FUNCTION__, __LINE__);
        return nullptr;
    }
    AlElement *e = new AlElement(name);
    for (TiXmlAttribute *attr = elm->FirstAttribute(); NULL != attr; attr = attr->Next()) {
        e->addAttr(attr->Name(), attr->Value());
    }
    for (TiXmlElement *ee = elm->FirstChildElement(); NULL != ee; ee = ee->NextSiblingElement()) {
        AlElement *child = _parseElement(ee);
        if (child) {
            e->addChild(child);
        }
    }
    return e;
}
