/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: aliminabc@gmail.com

* CREATE TIME: 2023-06-20 21:24:53
*/

#include "FkEngineMonitor.h"

FK_IMPL_CLASS_TYPE(IFkEngineMonitor, FkObject)
FK_IMPL_CLASS_TYPE(FkEngineMonitor, IFkEngineMonitor)

FkEngineMonitor::FkEngineMonitor() : IFkEngineMonitor() {

}

FkEngineMonitor::FkEngineMonitor(const FkEngineMonitor &o) : IFkEngineMonitor(o) {

}

FkEngineMonitor::~FkEngineMonitor() {

}