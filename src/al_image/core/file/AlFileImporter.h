/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALFILEIMPORTER_H
#define HWVC_ANDROID_ALFILEIMPORTER_H

#include "Object.h"
#include "AlImageLayerModel.h"
#include "AlImageCanvasModel.h"
#include "AlAbsOperateModel.h"
#include "tinyxml.h"

al_class(AlFileImporter) {
public:
    AlFileImporter();

    virtual ~AlFileImporter();

    HwResult importFromFile(std::string inFile,
                            AlImageCanvasModel *canvas,
                            std::vector<AlImageLayerModel *> *layers);

    HwResult importFromStr(std::string *inStr,
                           AlImageCanvasModel *canvas,
                           std::vector<AlImageLayerModel *> *layers);

private:
    AlFileImporter(const AlFileImporter &o) : Object() {}

    void _read(TiXmlDocument *doc, AlElement **element);

    AlElement *_parseElement(TiXmlElement *elm);
};


#endif //HWVC_ANDROID_ALFILEIMPORTER_H
