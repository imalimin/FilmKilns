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
#include "AlAbsMAction.h"
#include <map>

al_class(AlFileExporter) {
public:
    AlFileExporter();

    virtual ~AlFileExporter();

    /// 导出为文件
    /// \param canvas
    /// \param layers
    /// \param outPath 文件路径
    /// \return
    HwResult exportAsFile(AlImageCanvasModel *canvas,
                          std::vector<AlImageLayerModel *> *layers,
                          std::string outPath);

    /// 导出为字符串
    /// \param canvas
    /// \param layers
    /// \param outPath 文件路径
    /// \return
    HwResult exportAsStr(AlImageCanvasModel *canvas,
                         std::vector<AlImageLayerModel *> *layers,
                         std::string *outStr);

private:
    AlFileExporter(const AlFileExporter &o) : Object() {}
};


#endif //HWVC_ANDROID_ALFILEEXPORTER_H
