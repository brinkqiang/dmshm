#ifndef __LIBDMSINGLEIPC_IMPL_H_INCLUDE__
#define __LIBDMSINGLEIPC_IMPL_H_INCLUDE__

#include "dmsingleipc.h"
#include "dmshm.h"
// 引入新的、为共享内存设计的原子队列
#include "dmatomic_shm_queue.h"

/**
 * @brief IdmSingleIPC 接口的实现类。
 *
 * 内部实现已修改为使用 CDMSharedAtomicByteQueue。
 */
class DmSingleIPCImpl : public IdmSingleIPC
{
public:
    DmSingleIPCImpl();
    virtual ~DmSingleIPCImpl();

    // --- IdmSingleIPC interface ---
    virtual void DMAPI Release(void) override;
    virtual bool DMAPI Create(const std::string& name, size_t item_size, size_t item_count) override;
    virtual bool DMAPI Open(const std::string& name) override;
    virtual void DMAPI Close() override;
    virtual bool DMAPI Push(const void* data, size_t size) override;
    virtual bool DMAPI Pop(void* buffer, size_t buffer_size) override;

private:
    void internal_close();

private:
    CDMShmem                   m_shm;          // 底层共享内存包装器
    CDMSharedAtomicByteQueue* m_pQueue;       // 指向在共享内存中创建的队列对象
    std::string                m_shmName;      // 共享内存名称
    bool                       m_isCreator;    // 标记是否为创建者
};

#endif // __LIBDMSINGLEIPC_IMPL_H_INCLUDE__