/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFileExporter.h"
#include "AlXmlTagDefine.h"
#include "StringUtils.h"
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
    std::map<std::string, std::string> attrs;
    attrs.insert(Attr(VAL_VERSION, XML_VERSION));
    _writeTagStart(outStr, TAG_ROOT, &attrs);
    ///Write content
    _writeCanvas(outStr, canvas);
    for (int i = 0; i < layers->size(); ++i) {
        _writeLayer(outStr, (*layers)[i]);
    }
    ///Write content end
    _writeTagEnd(outStr, TAG_ROOT);
    return Hw::SUCCESS;
}

void AlFileExporter::_writeCanvas(std::string *str, AlImageCanvasModel *canvas) {
    std::map<std::string, std::string> attrs;
    attrs.insert(Attr(VAL_WIDTH, StringUtils::valueOf(canvas->getWidth())));
    attrs.insert(Attr(VAL_HEIGHT, StringUtils::valueOf(canvas->getHeight())));
    attrs.insert(Attr(VAL_COLOR, StringUtils::valueOf(canvas->getColor())));
    _writeTagStart(str, TAG_CANVAS, &attrs);
    _writeTagEnd(str, TAG_CANVAS);
}

void AlFileExporter::_writeTagStart(std::string *str, const char *tag,
                                    std::map<std::string, std::string> *attrs) {
    std::string attrsStr;
    auto itr = attrs->begin();
    while (attrs->end() != itr) {
        attrsStr.append(itr->first);
        attrsStr.append("=");
        attrsStr.append("\"");
        attrsStr.append(itr->second);
        attrsStr.append("\"");
        attrsStr.append("\n");
        ++itr;
    }
    str->append("<");
    str->append(tag);
    if (!attrsStr.empty()) {
        str->append(" ");
        str->append(attrsStr, 0, attrsStr.size() - 1);
    }
    str->append(">");
}

void AlFileExporter::_writeTagEnd(std::string *str, const char *tag) {
    str->append("</");
    str->append(tag);
    str->append(">");
}

void AlFileExporter::_writeLayer(std::string *str, AlImageLayerModel *layer) {
    std::map<std::string, std::string> attrs;
    attrs.insert(Attr(VAL_PATH, layer->getPath()));
    attrs.insert(Attr(VAL_ID, StringUtils::valueOf(layer->getId())));
    attrs.insert(Attr(VAL_ALPHA, StringUtils::valueOf(layer->getAlpha())));
    _writeTagStart(str, TAG_LAYER, &attrs);
    ///Scale
    attrs.clear();
    AlVec2 scale = layer->getScale();
    attrs.insert(Attr(VAL_VEC2_X, StringUtils::valueOf(scale.x)));
    attrs.insert(Attr(VAL_VEC2_Y, StringUtils::valueOf(scale.y)));
    _writeTagStart(str, TAG_SCALE, &attrs);
    _writeTagEnd(str, TAG_SCALE);
    ///Rotation
    attrs.clear();
    AlRational rotation = layer->getRotation();
    attrs.insert(Attr(VAL_RATIONAL_NUM, StringUtils::valueOf(rotation.num)));
    attrs.insert(Attr(VAL_RATIONAL_DEN, StringUtils::valueOf(rotation.den)));
    _writeTagStart(str, TAG_ROTATION, &attrs);
    _writeTagEnd(str, TAG_ROTATION);
    ///Position
    attrs.clear();
    AlVec2 pos = layer->getPosition();
    attrs.insert(Attr(VAL_VEC2_X, StringUtils::valueOf(pos.x)));
    attrs.insert(Attr(VAL_VEC2_Y, StringUtils::valueOf(pos.y)));
    _writeTagStart(str, TAG_POSITION, &attrs);
    _writeTagEnd(str, TAG_POSITION);

    _writeTagEnd(str, TAG_LAYER);
}
