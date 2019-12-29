/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFileExporter.h"
#include "AlXmlTagDefine.h"
#include "StringUtils.h"
#include "AlCropOperateModel.h"
#include "AlAlignCropOperateModel.h"
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
    size_t layerSize = layers->size();
    for (int i = 0; i < layerSize; ++i) {
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
    if (nullptr == layer) {
        return;
    }
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

    auto *opts = layer->getAllOperators();
    size_t optSize = opts->size();
    for (int i = 0; i < optSize; ++i) {
        auto *opt = (*opts)[i];
        _writeOpt(str, opt);
    }

    _writeTagEnd(str, TAG_LAYER);
}

void AlFileExporter::_writeOpt(std::string *str, AlAbsOperateModel *opt) {
    if (nullptr == opt) {
        return;
    }
    std::string type = opt->getType();
    AlSize canvasSize = opt->getCanvasSize();
    std::map<std::string, std::string> attrs;
    attrs.insert(Attr(VAL_TYPE, opt->getType()));
    attrs.insert(Attr(VAL_CANVAS_WIDTH, StringUtils::valueOf(canvasSize.width)));
    attrs.insert(Attr(VAL_CANVAS_HEIGHT, StringUtils::valueOf(canvasSize.height)));
    _writeTagStart(str, TAG_OPT, &attrs);
    if (StringUtils::equalsIgnoreCase(AlAbsOperateModel::TYPE_CROP, type)) {
        AlCropOperateModel *model = dynamic_cast<AlCropOperateModel *>(opt);
        AlRectF rectF = model->getRect();
        ///Rect
        attrs.clear();
        attrs.insert(Attr(VAL_LEFT, StringUtils::valueOf(rectF.left)));
        attrs.insert(Attr(VAL_TOP, StringUtils::valueOf(rectF.top)));
        attrs.insert(Attr(VAL_RIGHT, StringUtils::valueOf(rectF.right)));
        attrs.insert(Attr(VAL_BOTTOM, StringUtils::valueOf(rectF.bottom)));
        _writeTagStart(str, TAG_RECTF, &attrs);
        _writeTagEnd(str, TAG_RECTF);
        ///Scale
        attrs.clear();
        AlVec2 scale = model->getScale();
        attrs.insert(Attr(VAL_VEC2_X, StringUtils::valueOf(scale.x)));
        attrs.insert(Attr(VAL_VEC2_Y, StringUtils::valueOf(scale.y)));
        _writeTagStart(str, TAG_SCALE, &attrs);
        _writeTagEnd(str, TAG_SCALE);
        ///Rotation
        attrs.clear();
        AlRational rotation = model->getRotation();
        attrs.insert(Attr(VAL_RATIONAL_NUM, StringUtils::valueOf(rotation.num)));
        attrs.insert(Attr(VAL_RATIONAL_DEN, StringUtils::valueOf(rotation.den)));
        _writeTagStart(str, TAG_ROTATION, &attrs);
        _writeTagEnd(str, TAG_ROTATION);
        ///Position
        attrs.clear();
        AlVec2 pos = model->getPosition();
        attrs.insert(Attr(VAL_VEC2_X, StringUtils::valueOf(pos.x)));
        attrs.insert(Attr(VAL_VEC2_Y, StringUtils::valueOf(pos.y)));
        _writeTagStart(str, TAG_POSITION, &attrs);
        _writeTagEnd(str, TAG_POSITION);
    } else if (StringUtils::equalsIgnoreCase(AlAbsOperateModel::TYPE_ALIGN_CROP, type)) {
        AlAlignCropOperateModel *model = dynamic_cast<AlAlignCropOperateModel *>(opt);
        ///Rotation
        attrs.clear();
        AlRational rotation = model->getRotation();
        attrs.insert(Attr(VAL_RATIONAL_NUM, StringUtils::valueOf(rotation.num)));
        attrs.insert(Attr(VAL_RATIONAL_DEN, StringUtils::valueOf(rotation.den)));
        _writeTagStart(str, TAG_ROTATION, &attrs);
        _writeTagEnd(str, TAG_ROTATION);
    }
    _writeTagEnd(str, TAG_OPT);
}
