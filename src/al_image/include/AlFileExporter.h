/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALFILEEXPORTER_H
#define HWVC_ANDROID_ALFILEEXPORTER_H

#include "Object.h"
#include "AlImageLayerModel.h"
#include "AlImageCanvasModel.h"
#include "AlAbsOperateModel.h"

al_class(AlFileExporter) {
public:
    AlFileExporter();

    virtual ~AlFileExporter();

    HwResult export(AlImageCanvasModel *canvas, std::vector<AlImageLayerModel *> *layers);

private:
    AlFileExporter(const AlFileExporter &o) : Object() {}

    HwResult _exportXml(AlImageCanvasModel *canvas,
                        std::vector<AlImageLayerModel *> *layers,
                        std::string *outStr);

    HwResult _exportFile(AlImageCanvasModel *canvas,
                         std::vector<AlImageLayerModel *> *layers);

    void _writeHeader(std::string *str);

    void _writeFooter(std::string *str);

    void _writeCanvas(std::string *str, AlImageCanvasModel *canvas);

    void _writeTagStart(std::string *str, const char *tag, const char *attr = nullptr);

    void _writeTagEnd(std::string *str, const char *tag);

private:

};


#endif //HWVC_ANDROID_ALFILEEXPORTER_H
