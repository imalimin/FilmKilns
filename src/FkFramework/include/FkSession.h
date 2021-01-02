/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKSESSION_H
#define FK_FRAMEWORK_FKSESSION_H

#include "FkObject.h"
#include "FkProtocol.h"
#include "FkQuark.h"
#include <vector>

#define FK_CONNECT_TO(session, quark) (session->connectTo(std::static_pointer_cast<FkQuark>(quark)))

FK_CLASS FkSession FK_EXTEND FkObject {
public:
    template<class T>
    static std::shared_ptr<FkSession> with(std::shared_ptr<T> p) {
        auto session = std::make_shared<FkSession>();
        session->templateProtocol = std::static_pointer_cast<FkProtocol>(p);
        return session;
    }

public:
    FkSession();

    virtual ~FkSession();

    FkResult connectTo(const std::shared_ptr<FkQuark> quark);

    void close();

private:
    FkSession(const FkSession &o) : FkObject() {};

private:
    std::shared_ptr<FkProtocol> templateProtocol = nullptr;
    std::vector<std::shared_ptr<FkQuark>> link;
};


#endif //FK_FRAMEWORK_FKSESSION_H
