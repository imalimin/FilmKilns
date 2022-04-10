/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-3-8 12:58:51
*/

#ifndef FK_IMAGE_FKIMAGEFILEENGINE_H
#define FK_IMAGE_FKIMAGEFILEENGINE_H

#include "FkEngine.h"
#include "FkGraphicLayer.h"
#include "FkPictureModel.pb.h"
#include <functional>
#include <vector>

FK_CLASS FkImageModelEngine FK_EXTEND FkEngine {
public:
    typedef std::function<void(std::shared_ptr<fk_pb::FkPictureModel> &)> FkModelCallback;

public:
    FkImageModelEngine(std::shared_ptr<FkEngine> &imageEngine, std::string name);

    FkImageModelEngine(const FkImageModelEngine &o) = delete;

    virtual ~FkImageModelEngine();

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    FkResult save(std::string &file);

    FkResult load(std::string &file);

    FkResult getLayers(FkModelCallback callback);

    FkResult getLayer(FkID layer, FkModelCallback callback);

private:
    FkResult _save(std::shared_ptr<FkMessage> &msg);

    FkResult _load(std::shared_ptr<FkMessage> &msg);

    FkResult _getLayers(std::shared_ptr<FkMessage> &msg);

    FkResult _getLayer(std::shared_ptr<FkMessage> &msg);

    std::shared_ptr<fk_pb::FkPictureModel> convert2PictureModel(std::string &dir);

    FkResult _fillLayer(void* dst, std::shared_ptr<FkGraphicLayer> &src);

    std::string _createTempDir(std::string &file);

    FkResult _writeModel2File(std::string &dir, std::any model);

    FkResult _copyLayerFile(std::string &dir, std::string &src);

private:
    std::shared_ptr<FkEngine> imageEngine = nullptr;
    std::vector<std::shared_ptr<FkGraphicLayer>> layers;
};

#endif //FK_IMAGE_FKIMAGEFILEENGINE_H