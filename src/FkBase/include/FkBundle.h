/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef FK_BASE_FKBUNDLE_H
#define FK_BASE_FKBUNDLE_H

#include "FkObject.h"
#include <mutex>
#include <map>

FK_SUPER_CLASS(FkBundle, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkBundle)
public:
    FkBundle();

    FkBundle(const FkBundle &o);

    virtual ~FkBundle();

    bool put(std::string key, int32_t val);

    bool put(std::string key, int64_t val);

    bool put(std::string key, float val);

    bool put(std::string key, double val);

    bool put(std::string key, uint8_t val);

    bool put(std::string key, char val);

    bool put(std::string key, std::string val);

    int32_t get(std::string key, int32_t def);

    int64_t get(std::string key, int64_t def);

    float get(std::string key, float def);

    double get(std::string key, double def);

    uint8_t get(std::string key, uint8_t def);

    char get(std::string key, char def);

    std::string get(std::string key, std::string def);

    void remove(std::string key);

    bool contains(std::string key);

    virtual std::string toString() override;

private:
    std::map<std::string, std::shared_ptr<FkObject>> map;
    std::mutex mtx;
};


#endif //FK_BASE_FKBUNDLE_H
