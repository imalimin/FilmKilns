/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-28 22:08:01
*/

#ifndef FK_GRAPHIC_FKSCREENQUARK_H
#define FK_GRAPHIC_FKSCREENQUARK_H

#include "FkQuark.h"
#include "FkGraphicProgram.h"
#include "FkGraphicTexture.h"

FK_CLASS FkScreenQuark FK_EXTEND FkQuark {
public:
    FkScreenQuark();

    virtual ~FkScreenQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onRender(std::shared_ptr<FkProtocol> &p);

    FkResult _drawBackground(std::shared_ptr<FkProtocol> &p);

private:
    std::shared_ptr<FkGraphicProgramAllocator> allocator = nullptr;
    std::shared_ptr<FkGraphicAllocator> texAllocator = nullptr;
    std::shared_ptr<FkGraphicProgram> program = nullptr;
    std::shared_ptr<FkGraphicTexture> tex = nullptr;
};

#endif //FK_GRAPHIC_FKSCREENQUARK_H