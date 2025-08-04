#ifndef __DMSINGLEIPC_H_INCLUDE__
#define __DMSINGLEIPC_H_INCLUDE__

#include "dmos.h"
#include <string>
#include "dmmoduleptr.h"

// 前向声明，避免在头文件中暴露实现细节
class IdmSingleIPC;

/**
 * @brief IdmSingleIPC 模块的智能指针定义。
 * 使用 DmModulePtr，在 unique_ptr 销毁时会自动调用 Release() 方法。
 */
typedef DmModulePtr<IdmSingleIPC> dmsingleipcPtr;

/**
 * @brief 单生产者、单消费者进程间通信（IPC）接口类。
 *
 * 该接口提供了一个基于共享内存的FIFO队列。
 * 一个进程（生产者）通过 Create() 创建队列并可以 Push() 数据。
 * 另一个进程（消费者）通过 Open() 打开队列并可以 Pop() 数据。
 */
class IdmSingleIPC
{
public:
    virtual ~IdmSingleIPC() {}

    /**
     * @brief 释放模块实例。
     */
    virtual void DMAPI Release(void) = 0;

    /**
     * @brief (生产者端) 创建一个新的IPC通道。
     * * @param name 共享内存的唯一名称，用于跨进程识别。
     * @param item_size 每个数据项的固定大小（字节）。
     * @param item_count 队列能容纳的最大数据项数量。
     * @return bool true 如果创建成功，否则 false。
     */
    virtual bool DMAPI Create(const std::string& name, size_t item_size, size_t item_count) = 0;

    /**
     * @brief (消费者端) 打开一个已存在的IPC通道。
     * * @param name 要打开的共享内存的唯一名称。
     * @return bool true 如果打开成功，否则 false。
     */
    virtual bool DMAPI Open(const std::string& name) = 0;

    /**
     * @brief 关闭IPC通道。
     * 对于创建者，这将会释放底层的共享内存。
     * 对于打开者，这只会断开连接。
     */
    virtual void DMAPI Close() = 0;

    /**
     * @brief (生产者端)向队列中推入一个数据项。
     * 这是一个非阻塞操作。
     * * @param data 指向要推入数据的指针。
     * @param size 要推入数据的大小，必须等于创建时指定的 item_size。
     * @return bool true 如果成功推入，false 如果队列已满。
     */
    virtual bool DMAPI Push(const void* data, size_t size) = 0;

    /**
     * @brief (消费者端)从队列中弹出一个数据项。
     * 这是一个非阻塞操作。
     * * @param buffer 指向用于接收数据的缓冲区的指针。
     * @param buffer_size 接收缓冲区的大小，必须大于或等于 item_size。
     * @return bool true 如果成功弹出，false 如果队列为空。
     */
    virtual bool DMAPI Pop(void* buffer, size_t buffer_size) = 0;
};

/**
 * @brief 获取 IdmSingleIPC 模块的实例。
 * @return 指向 IdmSingleIPC 实例的指针。
 */
extern "C" DMEXPORT_DLL IdmSingleIPC* DMAPI dmSingleIPCGetModule();
typedef IdmSingleIPC* (DMAPI* PFN_dmSingleIPCGetModule)();

#endif // __DMSINGLEIPC_H_INCLUDE__