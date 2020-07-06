/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "AlFuture.h"

#define TAG "AlFuture"
#define AL_SYNC_BUNDLE_PUT \
  std::unique_lock<std::mutex> lck(mtx); \
  bool ret = bundle.put(TAG, val); \
  done = true; \
  cond.notify_all(); \
  return ret; \

#define AL_SYNC_BUNDLE_GET \
  std::unique_lock<std::mutex> lck(mtx); \
  if (done) { \
      return bundle.get(TAG, def); \
  } \
  cond.wait(lck); \
  return bundle.get(TAG, def); \


AlFuture::AlFuture() : Object() {

}

AlFuture::~AlFuture() {

}

bool AlFuture::putInt(int32_t val) {
    std::unique_lock<std::mutex> lck(mtx);
    bool ret = bundle.put(TAG, val);
    done = true;
    cond.notify_all();
    return ret;
}

bool AlFuture::put(int64_t val) {
    AL_SYNC_BUNDLE_PUT
}

bool AlFuture::put(float val) {
    AL_SYNC_BUNDLE_PUT
}

bool AlFuture::put(double val) {
    AL_SYNC_BUNDLE_PUT
}

bool AlFuture::put(uint8_t val) {
    AL_SYNC_BUNDLE_PUT
}

bool AlFuture::put(char val) {
    AL_SYNC_BUNDLE_PUT
}

bool AlFuture::put(std::string val) {
    AL_SYNC_BUNDLE_PUT
}

int32_t AlFuture::get(int32_t def) {
    AL_SYNC_BUNDLE_GET
}

int64_t AlFuture::get(int64_t def) {
    AL_SYNC_BUNDLE_GET
}

float AlFuture::get(float def) {
    AL_SYNC_BUNDLE_GET
}

double AlFuture::get(double def) {
    AL_SYNC_BUNDLE_GET
}

uint8_t AlFuture::get(uint8_t def) {
    AL_SYNC_BUNDLE_GET
}

char AlFuture::get(char def) {
    AL_SYNC_BUNDLE_GET
}

std::string AlFuture::get(std::string def) {
    AL_SYNC_BUNDLE_GET
}