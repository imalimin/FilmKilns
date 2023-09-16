//
// Created by lmy on 2023/9/14.
//
#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <boost/stacktrace.hpp>

namespace fk_hook {
    typedef struct Node_ {
        size_t byteSize = 0;
        size_t hash = 0;
        boost::stacktrace::stacktrace stacktrace;
        std::vector<std::string> strStacktrace;
    } Node;

    typedef std::unordered_map<void *, Node> NodeMap;
}

class FkHook {

public:
    static FkHook &getInstance();

    FkHook();

    FkHook(const FkHook &o) = delete;

    virtual ~FkHook();

    int start();

    int stop();

    int clear();

    int dump(int64_t &leakSize, int &leakCount);

    int dump(fk_hook::NodeMap &map);

private:
    static std::unique_ptr<FkHook> instance;
};
