/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICPROGRAMQUARK_H
#define FK_GRAPHIC_FKGRAPHICPROGRAMQUARK_H

#include "FkQuark.h"
#include "FkGraphicProgram.h"

class FkGraphicProgramQuark FK_EXTEND FkQuark {
public:
    FkGraphicProgramQuark();

    FkGraphicProgramQuark(const FkGraphicProgramQuark &o) = delete;

    virtual ~FkGraphicProgramQuark();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    FkResult _onDrawLayer(std::shared_ptr<FkProtocol> p);

    FkResult _fillValue(std::shared_ptr<FkGraphicProgram> program);

private:
    std::shared_ptr<FkGraphicProgramAllocator> allocator = nullptr;

};

#endif //FK_GRAPHIC_FKGRAPHICPROGRAMQUARK_H
