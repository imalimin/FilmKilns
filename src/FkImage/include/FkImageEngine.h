/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_IMAGE_FKIMAGEENGINE_H
#define FK_IMAGE_FKIMAGEENGINE_H

#include "FkObject.h"
#include "FkLayerEngine.h"

FK_CLASS FkImageEngine FK_EXTEND FkLayerEngine {
public:
    FkImageEngine(std::shared_ptr<FkEngine> &renderEngine, std::string &workspace, std::string name);

    FkImageEngine(const FkImageEngine &o) = delete;

    virtual ~FkImageEngine();

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    FkID newLayerWithFile(std::string path, FkID expectId = FK_ID_NONE);

private:
    FkResult _updateLayerWithFile(std::shared_ptr<FkMessage> msg);

private:
    static const FkID FK_MSG_UPDATE_LAYER_WITH_FILE;
    std::string workspace;
};

#endif //FK_IMAGE_FKIMAGEENGINE_H