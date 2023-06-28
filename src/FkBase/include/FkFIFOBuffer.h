/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_BASE_FKFIFOBUFFER_H
#define FK_BASE_FKFIFOBUFFER_H

#include "FkObject.h"
#include "FkBuffer.h"
#include "FkSimpleLock.h"
#include <mutex>

FK_SUPER_CLASS(FkFIFOBuffer, FkObject) {
FK_DEF_CLASS_TYPE_FUNC(FkFIFOBuffer)

public:
    FkFIFOBuffer(size_t capacity);

    virtual ~FkFIFOBuffer();

    /**
     * 把数据写入fifo
     * @data 数据指针
     * @size data的大小
     * @return 返回成功push的size，0表示失败
     */
    size_t push(uint8_t *data, size_t size);

    size_t push(uint8_t *data, size_t size, int32_t timeOut);

    /**
     * 从fifo读取数据
     * @size 期望得到的size
     * @return 返回数据片段映射，大小小于或等于size，该内存片段由fifo维护，切勿进行写操作
     */
    std::shared_ptr<FkBuffer> take(size_t size);

    std::shared_ptr<FkBuffer> take(size_t size, int32_t timeOut);

    size_t size();

    void flush();

private:
    uint8_t *first();

    uint8_t *end();

    size_t leftCapacity();

    bool willWrite(size_t size);

    bool willRead(size_t size);

    uint8_t *move(uint8_t *pointer, size_t size);

    void printBufferState();

private:
    uint8_t *buf = nullptr;
    size_t capacity = 0;
    size_t _size = 0;
    uint8_t *reader = nullptr;
    uint8_t *writer = nullptr;
    uint8_t *endFlag = nullptr;

    FkSimpleLock pushLock;
    FkSimpleLock takeLock;
    std::mutex mutex;
    bool requestFlush = false;
};


#endif //FK_BASE_FKFIFOBUFFER_H
