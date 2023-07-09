/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-1 23:51:27
*/

#include "FkEngineContext.h"

FK_IMPL_CLASS_TYPE(FkEngineContext, FkEntity)

FkEngineContext::FkEngineContext() : FkEntity() {

}

FkEngineContext::~FkEngineContext() {

}

void FkEngineContext::setEngineSettings(std::shared_ptr<FkEngineSettings> &_settings) {
    this->settings = _settings;
}

std::shared_ptr<FkEngineSettings> FkEngineContext::getEngineSettings() {
    return this->settings;
}

void FkEngineContext::setMonitor(std::shared_ptr<FkAbsEngineMonitor> &_monitor) {
    this->monitor = _monitor;
}

std::shared_ptr<FkAbsEngineMonitor> FkEngineContext::getMonitor() {
    return monitor;
}