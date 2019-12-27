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

AlFileExporter::AlFileExporter() : Object() {

}

AlFileExporter::~AlFileExporter() {

}

HwResult AlFileExporter::export(AlImageCanvasModel *canvas,
                                std::vector<AlImageLayerModel *> *layers) {
    return _exportFile(canvas, layers);
}

HwResult AlFileExporter::_exportXml(AlImageCanvasModel *canvas,
                                    std::vector<AlImageLayerModel *> *layers,
                                    std::string *outStr) {
    if (nullptr == canvas || nullptr == layers || layers->empty() || nullptr == outStr) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    _writeHeader(outStr);
    _writeCanvas(outStr, canvas);
    _writeFooter(outStr);
    return Hw::SUCCESS;
}

HwResult AlFileExporter::_exportFile(AlImageCanvasModel *canvas,
                                     std::vector<AlImageLayerModel *> *layers) {
    std::string xmlStr;
    HwResult ret = _exportXml(canvas, layers, &xmlStr);
    if (Hw::SUCCESS != ret) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    FILE *out = fopen("/sdcard/000.xml", "wb");
    if (nullptr == out) {
        Logcat::i(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return Hw::FAILED;
    }
    fwrite(xmlStr.c_str(), xmlStr.size(), 1, out);
    fclose(out);
    return Hw::SUCCESS;
}

void AlFileExporter::_writeHeader(std::string *str) {
    str->append("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
    str->append("<image version=\"");
    str->append(XML_VERSION);
    str->append("\">");
}

void AlFileExporter::_writeFooter(std::string *str) {
    str->append("</image>");
}

void AlFileExporter::_writeCanvas(std::string *str, AlImageCanvasModel *canvas) {
    std::string attr;
    ///Width
    attr.append(VAL_WIDTH);
    attr.append("=");
    attr.append("\"");
    attr.append(StringUtils::valueOf(canvas->getWidth()));
    attr.append("\"");
    ///Height
    attr.append(VAL_HEIGHT);
    attr.append("=");
    attr.append("\"");
    attr.append(StringUtils::valueOf(canvas->getHeight()));
    attr.append("\"");
    ///Color
    attr.append(VAL_COLOR);
    attr.append("=");
    attr.append("\"");
    attr.append(StringUtils::valueOf(canvas->getColor()));
    attr.append("\"");
    _writeTagStart(str, TAG_CANVAS);

    _writeTagEnd(str, TAG_CANVAS);
}

void AlFileExporter::_writeTagStart(std::string *str, const char *tag, const char *attr) {
    str->append("<");
    str->append(tag);
    if (attr) {
        str->append(" ");
        str->append(attr);
    }
    str->append(">");
}

void AlFileExporter::_writeTagEnd(std::string *str, const char *tag) {
    str->append("</");
    str->append(tag);
    str->append(">");
}
