/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-4-9 23:59:09
*/

#include "FkAnyCompo.h"

FkAnyCompo::FkAnyCompo(std::any any) : FkObject(), any(std::move(any)) {
    FK_MARK_SUPER
}

FkAnyCompo::FkAnyCompo(const FkAnyCompo &o) : FkObject(o), any(o.any) {
    FK_MARK_SUPER
}

FkAnyCompo::~FkAnyCompo() {
    any = nullptr;
}