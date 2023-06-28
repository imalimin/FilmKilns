/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-27 13:47:11
*/

#ifndef FK_GRAPHIC_FKRENDERPROGRAMQUARK_H
#define FK_GRAPHIC_FKRENDERPROGRAMQUARK_H

#include "FkQuark.h"
#include "FkGraphicProgram.h"

FK_SUPER_CLASS(FkRenderProgramQuark, FkQuark) {
FK_DEF_CLASS_TYPE_FUNC(FkRenderProgramQuark)

public:
    FkRenderProgramQuark();

    virtual ~FkRenderProgramQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onRender(std::shared_ptr<FkProtocol> p);

private:
    std::shared_ptr<FkGraphicProgramAllocator> allocator = nullptr;
};

#endif //FK_GRAPHIC_FKRENDERPROGRAMQUARK_H