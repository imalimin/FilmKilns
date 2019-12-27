/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFileImporter.h"
#include "AlXmlTagDefine.h"
#include "StringUtils.h"
#include "Logcat.h"
#include "tinyxml.h"

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
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    TiXmlElement *root = doc.FirstChildElement();
    if (nullptr == root) {
        doc.Clear();
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    string rootTag = root->Value();
    if (!StringUtils::endsWith(TAG_ROOT, rootTag)) {
        doc.Clear();
        Logcat::i(TAG, "%s(%d) invalid qua file.", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    const char *version = root->Attribute(VAL_VERSION);
    Logcat::i(TAG, "%s(%d) .alx version ", __FUNCTION__, __LINE__, version);
    for (TiXmlElement *elem = root->FirstChildElement();
         elem != NULL; elem = elem->NextSiblingElement()) {
        if (Hw::SUCCESS != _parseElement(elem, canvas, layers)) {
            doc.Clear();
            Logcat::i(TAG, "%s(%d) invalid qua file.", __FUNCTION__, __LINE__);
            return Hw::FAILED;
        }
    }
    doc.Clear();
    return Hw::SUCCESS;
}

HwResult AlFileImporter::importFromStr(std::string *inStr, AlImageCanvasModel *canvas,
                                       std::vector<AlImageLayerModel *> *layers) {
    return Hw::SUCCESS;
}

HwResult AlFileImporter::_parseElement(void *e, AlImageCanvasModel *canvas,
                                       std::vector<AlImageLayerModel *> *layers) {
    if (nullptr == e) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    TiXmlElement *elm = static_cast<TiXmlElement *>(e);
    std::string name = elm->Value();
    if (StringUtils::endsWith(TAG_CANVAS, name)) {
        const char *str = elm->Attribute(VAL_WIDTH);
        int32_t width = StringUtils::toInt(str);
        str = elm->Attribute(VAL_HEIGHT);
        int32_t height = StringUtils::toInt(str);
        str = elm->Attribute(VAL_COLOR);
        int32_t color = StringUtils::toInt(str);
        if (width <= 0 || height <= 0) {
            Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
            return Hw::FAILED;
        }
        canvas->set(width, height, color);
    } else if (StringUtils::endsWith(TAG_LAYER, name)) {
        const char *str = elm->Attribute(VAL_PATH);
        std::string path = str;
        str = elm->Attribute(VAL_ID);
        int32_t id = StringUtils::toInt(str);
        str = elm->Attribute(VAL_ALPHA);
        float alpha = StringUtils::toFloat(str);
        if (StringUtils::isEmpty(&path) || alpha > 1 || alpha < -1) {
            Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
            return Hw::FAILED;
        }
        auto *layer = AlImageLayerModel::create(id, path);
        layer->setAlpha(alpha);
        layers->push_back(layer);

        for (TiXmlElement *elem = elm->FirstChildElement();
             elem != NULL; elem = elem->NextSiblingElement()) {
            std::string tagName = elem->Value();
            if (StringUtils::endsWith(TAG_SCALE, tagName)) {
                str = elem->Attribute(VAL_VEC2_X);
                float x = StringUtils::toFloat(str);
                str = elem->Attribute(VAL_VEC2_Y);
                float y = StringUtils::toFloat(str);
                if (MAXFLOAT != x || MAXFLOAT != y) {
                    layer->setScale(x, y);
                }
            } else if (StringUtils::endsWith(TAG_ROTATION, tagName)) {
                str = elem->Attribute(VAL_RATIONAL_NUM);
                int32_t num = StringUtils::toInt(str);
                str = elem->Attribute(VAL_RATIONAL_DEN);
                int32_t den = StringUtils::toInt(str);
                if (INT32_MIN != num || INT32_MIN != den) {
                    AlRational r(num, den);
                    layer->setRotation(r);
                }
            } else if (StringUtils::endsWith(TAG_POSITION, tagName)) {
                str = elem->Attribute(VAL_VEC2_X);
                float x = StringUtils::toFloat(str);
                str = elem->Attribute(VAL_VEC2_Y);
                float y = StringUtils::toFloat(str);
                if (MAXFLOAT != x || MAXFLOAT != y) {
                    layer->setPosition(x, y);
                }
            }
        }
    }

    return Hw::SUCCESS;
}
