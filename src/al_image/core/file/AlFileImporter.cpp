/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFileImporter.h"
#include "AlXmlTagDefine.h"
#include "AlLayerActionFactory.h"
#include "AlMCropAction.h"
#include "StringUtils.h"
#include "AlObjectGuard.h"
#include "AlXMLDocument.h"
#include "Logcat.h"

#define TAG "AlFileImporter"

AlFileImporter::AlFileImporter() : Object() {

}

AlFileImporter::~AlFileImporter() {

}

HwResult AlFileImporter::importFromFile(std::string inFile, AlImageCanvasModel *canvas,
                                        std::vector<AlImageLayerModel *> *layers) {
    AlXMLDocument *doc = AlXMLDocument::fromFile(inFile);
    AlObjectGuard guard((Object **) &doc);
    if (nullptr == doc) {
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    AlElement *root = doc->root();
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
