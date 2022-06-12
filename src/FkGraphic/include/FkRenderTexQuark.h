/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:17:35
*/

#ifndef FK_GRAPHIC_FKRENDERTEXQUARK_H
#define FK_GRAPHIC_FKRENDERTEXQUARK_H

#include "FkQuark.h"
#include "FkGraphicTexture.h"
#include "FkTexEntity.h"
#include "FkIntVec2.h"
#include "FkBuffer.h"
#include "FkTexArrayCompo.h"

FK_SUPER_CLASS(FkRenderTexQuark, FkQuark) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderTexQuark)

public:
    FkRenderTexQuark();

    FkRenderTexQuark(const FkRenderTexQuark &o) = delete;

    virtual ~FkRenderTexQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onAllocTex(std::shared_ptr<FkProtocol> &p);

    FkResult _onAllocTexWithBmp(std::shared_ptr<FkProtocol> &p);

    FkResult _onRender(std::shared_ptr<FkProtocol> &p);

    FkResult _onRemoveTex(std::shared_ptr<FkProtocol> &p);

private:
    std::shared_ptr<FkTexArrayCompo> _findTex(FkID id);

    std::shared_ptr<FkTexArrayCompo> _allocTex(std::shared_ptr<FkTexEntity> &texEntity);

    FkResult _updateTexWithBuf(std::shared_ptr<FkTexArrayCompo> &texArray,
                               std::shared_ptr<FkBuffer> &buf,
                               std::shared_ptr<FkTexEntity> &texEntity);

    FkResult _drawColor(std::shared_ptr<FkTexArrayCompo> &texArray,
                        std::shared_ptr<FkTexEntity> &texEntity);

    FkResult _drawColor(std::shared_ptr<FkGraphicTexture> &tex,
                        std::shared_ptr<FkTexEntity> &texEntity);

    FkIntVec2 _calcTexBlock(FkSize size, int32_t blockSize);

    FkResult _copySubImage(std::shared_ptr<FkBuffer> &src, int32_t width,
                           std::shared_ptr<FkBuffer> &dst, FkSize size,
                           FkIntVec2 pos);

private:
    std::shared_ptr<FkGraphicAllocator> allocator = nullptr;
    std::unordered_map<FkID, std::shared_ptr<FkTexArrayCompo>> sMap;
};

#endif //FK_GRAPHIC_FKRENDERTEXQUARK_H