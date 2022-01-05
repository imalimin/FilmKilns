/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICVBOQUARK_H
#define FK_GRAPHIC_FKGRAPHICVBOQUARK_H

#include "FkQuark.h"
#include "FkVertexObject.h"

FK_CLASS FkGraphicVBOQuark FK_EXTEND FkQuark {
public:
    FkGraphicVBOQuark();

    FkGraphicVBOQuark(const FkGraphicVBOQuark &o) = delete;

    virtual ~FkGraphicVBOQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p);

private:
    std::shared_ptr<FkVertexObjectAllocator> allocator = nullptr;
};

#endif //FK_GRAPHIC_FKGRAPHICVBOQUARK_H