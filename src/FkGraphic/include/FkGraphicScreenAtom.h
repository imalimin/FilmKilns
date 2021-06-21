/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICSCREENATOM_H
#define FK_GRAPHIC_FKGRAPHICSCREENATOM_H

#include "FkSimpleAtom.h"
#include "FkLocalClient.h"
#include "FkGraphicProgram.h"

FK_CLASS FkGraphicScreenAtom FK_EXTEND FkSimpleAtom {
public:
    FkGraphicScreenAtom();

    FkGraphicScreenAtom(const FkGraphicScreenAtom &o) = delete;

    virtual ~FkGraphicScreenAtom();

protected:
    virtual void describeProtocols(std::shared_ptr<FkPortDesc> desc) override;

    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

    virtual void onConnect(std::shared_ptr<FkConnectChain> chain) override;

private:
    FkResult _onRenderRequest(std::shared_ptr<FkProtocol> p);

private:
    float *position = nullptr;
    float *coordinate = nullptr;
    uint32_t vbo = 0;
};


#endif //FK_GRAPHIC_FKGRAPHICSCREENATOM_H
