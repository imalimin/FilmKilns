//
// Created by lmy on 2023/9/14.
//
#pragma once

#include <memory>
#include <unordered_map>

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

private:
    static std::unique_ptr<FkHook> instance;
};
