/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-3-1 23:51:27
*/

#ifndef FK_FRAMEWORK_FKQUARKCONTEXT_H
#define FK_FRAMEWORK_FKQUARKCONTEXT_H

#include "FkEntity.h"
#include "FkEngineSettings.h"
#include "FkEngineMonitor.h"

FK_SUPER_CLASS(FkEngineContext, FkEntity) {
FK_DEF_CLASS_TYPE_FUNC(FkEngineContext)

public:
    FkEngineContext();

    FkEngineContext(const FkEngineContext &o) = delete;

    virtual ~FkEngineContext();

    virtual void setEngineSettings(std::shared_ptr<FkEngineSettings> &_settings);

    virtual std::shared_ptr<FkEngineSettings> getEngineSettings();

    virtual void setMonitor(std::shared_ptr<FkAbsEngineMonitor> &_monitor);

    virtual std::shared_ptr<FkAbsEngineMonitor> getMonitor();

private:
    std::shared_ptr<FkEngineSettings> settings;
    std::shared_ptr<FkAbsEngineMonitor> monitor = nullptr;
};

#endif //FK_FRAMEWORK_FKQUARKCONTEXT_H