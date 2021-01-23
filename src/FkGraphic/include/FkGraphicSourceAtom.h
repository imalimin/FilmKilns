/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_GRAPHIC_FKGRAPHICSOURCEATOM_H
#define FK_GRAPHIC_FKGRAPHICSOURCEATOM_H

#include "FkAtom.h"
#include "FkLocalClient.h"

FK_CLASS FkGraphicSourceAtom FK_EXTEND FkAtom {
public:
    FkGraphicSourceAtom();

    FkGraphicSourceAtom(const FkGraphicSourceAtom &o) = delete;

    virtual ~FkGraphicSourceAtom();

protected:
    virtual FkResult onCreate() override;

    virtual FkResult onDestroy() override;

    virtual FkResult onStart() override;

    virtual FkResult onStop() override;

private:
    std::shared_ptr<FkSessionClient> client;
};


#endif //FK_GRAPHIC_FKGRAPHICSOURCEATOM_H
