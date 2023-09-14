//
// Created by lmy on 2023/9/14.
//

#include "FkHook.h"
#include "xhook.h"
#include <mutex>
#include <vector>
#include <unordered_map>

#define TAG "FkHook"

static std::mutex fk_hook_mutex;
static std::unordered_map<void *, size_t> fk_hook_map;

static void *(*malloc_)(size_t) = nullptr;

static void *(*calloc_)(size_t, size_t) = nullptr;

static void *(*realloc_)(void *, size_t) = nullptr;

static void (*free_)(void *) = nullptr;

static void *(*_Znwm_)(unsigned long) = nullptr;

static void *(*_ZnwmRKSt9nothrow_t_)(unsigned long) = nullptr;

static void *(*_Znam_)(unsigned long) = nullptr;

static void *(*_ZnamRKSt9nothrow_t_)(unsigned long) = nullptr;

static void (*_ZdlPv_)(void *) = nullptr;

static void (*_ZdlPvRKSt9nothrow_t_)(void *) = nullptr;

static void (*_ZdaPv_)(void *) = nullptr;

static void (*_ZdaPvRKSt9nothrow_t_)(void *) = nullptr;

static void (*_ZdlPvS__)(void *, void *) = nullptr;

static void (*_ZdaPvS__)(void *, void *) = nullptr;

static void fk_put_mem_map(void *ptr, size_t size) {
    if (ptr == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> guard(fk_hook_mutex);
    auto itr = fk_hook_map.find(ptr);
    if (itr != fk_hook_map.end()) {
        fk_hook_map.erase(itr);
        fk_hook_map.insert(std::make_pair(ptr, size));
    } else {
        fk_hook_map.insert(std::make_pair(ptr, size));
    }
}

static void fk_remove_mem_map(void *ptr) {
    if (ptr == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> guard(fk_hook_mutex);
    auto itr = fk_hook_map.find(ptr);
    if (itr != fk_hook_map.end()) {
        fk_hook_map.erase(itr);
    }
}

void *fk_proxy_malloc(size_t __byte_count) {
    void *ptr = malloc_(__byte_count);
    fk_put_mem_map(ptr, __byte_count);
    return ptr;
}

void *fk_proxy_calloc(size_t __item_count, size_t __item_size) {
    void *ptr = calloc_(__item_count, __item_size);
    fk_put_mem_map(ptr, __item_count * __item_size);
    return ptr;
}

void *fk_proxy_realloc(void *__ptr, size_t __byte_count) {
    void *ptr = realloc_(__ptr, __byte_count);
    fk_put_mem_map(ptr, __byte_count);
    return ptr;
}

void fk_proxy_free(void *__ptr) {
    free_(__ptr);
    fk_remove_mem_map(__ptr);
}

void *fk_proxy_new(unsigned long __byte_count) {
    void *ptr = _Znwm_(__byte_count);
    fk_put_mem_map(ptr, __byte_count);
    return ptr;
}

void *fk_proxy_new_nothrow(unsigned long __byte_count) {
    void *ptr = _ZnwmRKSt9nothrow_t_(__byte_count);
    fk_put_mem_map(ptr, __byte_count);
    return ptr;
}

void *fk_proxy_new_arr(unsigned long __byte_count) {
    void *ptr = _Znam_(__byte_count);
    fk_put_mem_map(ptr, __byte_count);
    return ptr;
}

void *fk_proxy_new_arr_nothrow(unsigned long __byte_count) {
    void *ptr = _ZnamRKSt9nothrow_t_(__byte_count);
    fk_put_mem_map(ptr, __byte_count);
    return ptr;
}

void fk_proxy_delete(void *__ptr) {
    _ZdlPv_(__ptr);
    fk_remove_mem_map(__ptr);
}

void fk_proxy_delete_nothrow(void *__ptr) {
    _ZdlPvRKSt9nothrow_t_(__ptr);
    fk_remove_mem_map(__ptr);
}

void fk_proxy_delete_arr(void *__ptr) {
    _ZdaPv_(__ptr);
    fk_remove_mem_map(__ptr);
}

void fk_proxy_delete_arr_nothrow(void *__ptr) {
    _ZdaPvRKSt9nothrow_t_(__ptr);
    fk_remove_mem_map(__ptr);
}

std::unique_ptr<FkHook> FkHook::instance = std::unique_ptr<FkHook>();

FkHook &FkHook::getInstance() {
    return *instance;
}

FkHook::FkHook() {
}

FkHook::~FkHook() {
}

int FkHook::start() {
    clear();
    xhook_enable_sigsegv_protection(0);
//#define FK_HOOK_REGEX ".*FkAndroid$|FilmKilns|webrtcmaster|hwffmpeg"
    std::vector<std::string> arr = {".*/libFkAndroid.so$",
                                    ".*/libFilmKilns.so$",
                                    ".*/libwebrtcmaster.so$",
                                    ".*/libhwffmpeg.so$"};
    for(std::string &it : arr) {
        xhook_register(it.c_str(), "malloc", (void *) fk_proxy_malloc, (void **) &malloc_);
        xhook_register(it.c_str(), "calloc", (void *) fk_proxy_calloc, (void **) &calloc_);
        xhook_register(it.c_str(), "realloc", (void *) fk_proxy_realloc, (void **) &realloc_);
        xhook_register(it.c_str(), "free", (void *) fk_proxy_free, (void **) &free_);

        xhook_register(it.c_str(), "_Znwm", (void *) fk_proxy_new, (void **) &_Znwm_);
        xhook_register(it.c_str(), "_ZnwmRKSt9nothrow_t", (void *) fk_proxy_new_nothrow,
                       (void **) &_ZnwmRKSt9nothrow_t_);
        xhook_register(it.c_str(), "_Znam", (void *) fk_proxy_new_arr, (void **) &_Znam_);
        xhook_register(it.c_str(), "_ZnamRKSt9nothrow_t", (void *) fk_proxy_new_arr_nothrow,
                       (void **) &_ZnamRKSt9nothrow_t_);

        xhook_register(it.c_str(), "_ZdlPv", (void *) fk_proxy_delete, (void **) &_ZdlPv_);
        xhook_register(it.c_str(), "_ZdlPvRKSt9nothrow_t", (void *) fk_proxy_delete_nothrow,
                       (void **) &_ZdlPvRKSt9nothrow_t_);
        xhook_register(it.c_str(), "_ZdaPv", (void *) fk_proxy_delete_arr, (void **) &_ZdaPv_);
        xhook_register(it.c_str(), "_ZdaPvRKSt9nothrow_t", (void *) fk_proxy_delete_arr_nothrow,
                       (void **) &_ZdaPvRKSt9nothrow_t_);

//    xhook_register(FK_HOOK_REGEX, "_ZdlPvS_", (void *) fk_proxy_new, (void **) &_ZdlPvS__);
//    xhook_register(FK_HOOK_REGEX, "_ZdaPvS_", (void *) fk_proxy_new, (void **) &_ZdaPvS__);
    }
    return xhook_refresh(0);
}

int FkHook::stop() {
    xhook_clear();
    return 0;
}

int FkHook::clear() {
    std::lock_guard<std::mutex> guard(fk_hook_mutex);
    fk_hook_map.clear();
    return 0;
}


int FkHook::dump(int64_t &leakSize, int &leakCount) {
    std::lock_guard<std::mutex> guard(fk_hook_mutex);
    leakSize = 0;
    leakCount = fk_hook_map.size();
    for (auto &itr: fk_hook_map) {
        leakSize += itr.second;
    }
    return 0;
}