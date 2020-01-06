/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFileExporter.h"
#include "AlXmlTagDefine.h"
#include "StringUtils.h"
#include "AlMCropAction.h"
#include "AlMAlignCropAction.h"
#include "AlElement.h"
#include "AlObjectGuard.h"
#include "Logcat.h"

#define TAG "AlFileExporter"
#define Attr std::pair<std::string, std::string>

AlFileExporter::AlFileExporter() : Object() {

}

AlFileExporter::~AlFileExporter() {

}

HwResult AlFileExporter::exportAsFile(AlImageCanvasModel *canvas,
                                      std::vector<AlImageLayerModel *> *layers,
                                      std::string outPath) {
    if (StringUtils::isEmpty(&outPath)) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    std::string xmlStr;
    HwResult ret = exportAsStr(canvas, layers, &xmlStr);
    if (Hw::SUCCESS != ret) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    FILE *out = fopen(outPath.c_str(), "wb");
    if (nullptr == out) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    fwrite(xmlStr.c_str(), xmlStr.size(), 1, out);
    fclose(out);
    return Hw::SUCCESS;
}

HwResult AlFileExporter::exportAsStr(AlImageCanvasModel *canvas,
                                     vector<AlImageLayerModel *> *layers,
                                     std::string *outStr) {
    if (nullptr == canvas || nullptr == layers || layers->empty() || nullptr == outStr) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    outStr->append("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
    outStr->append("\n");

    AlElement *root = new AlElement(TAG_ROOT);
    AlObjectGuard guard((Object **) &root);
    root->addAttr(VAL_VERSION, XML_VERSION);
    AlElement *cEle = nullptr;
    canvas->toElement(&cEle);
    if (nullptr == cEle) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    root->addChild(cEle);

    size_t size = layers->size();
    for (int i = 0; i < size; ++i) {
        AlElement *e = nullptr;
        (*layers)[i]->toElement(&e);
        if (nullptr == e) {
            Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
            return Hw::FAILED;
        }
        root->addChild(e);
    }
    outStr->append(root->toString());
    return Hw::SUCCESS;
}
